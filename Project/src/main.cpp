/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/22 17:03:15 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Tools.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "PrivmsgCommand.hpp"
#include "Tools.hpp"
#include "Colors.hpp"
#include "JoinCommand.hpp"
#include "Parser.hpp"
#include "Command.hpp"
#include "CommandDispatcher.hpp"
#include <iostream>
/*
Your executable will be run as follows:
./ircserv <port> <password>
*/


//https://www.suchprogramming.com/epoll-in-3-easy-steps/
/*
	MAX_EVENTS → how many FDs epoll_wait will return at once (not max clients).
	BUFFER_SIZE → how many bytes you read from a socket at once.
*/
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024

void	setNonBlocking(int sv_fd)
{
	int flag = fcntl(sv_fd, F_GETFL, 0);
	if (flag == -1)
	{
		std::cout << "fcntl F_GETFL error" << std::endl;
		std::exit(-1);
	}
	if (fcntl(sv_fd, F_SETFL, flag | O_NONBLOCK) == -1)
	{
		std::cout << "fcntl F_SETFL error" << std::endl;
		std::exit(-1);
	}
}


void cleanupClient(Server &s, int fd)
{
	epoll_ctl(s.get_epollFD(), EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	s.rmClientMap(fd);
}



void handleNewConnection(Server &s)
{
	while (true)
	{
		int cl_fd = accept(s.get_serverFD(), NULL, NULL);
		if (cl_fd < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			std::cout << "accept error" << std::endl;
			return;
		}
		setNonBlocking(cl_fd);
		struct epoll_event ev;
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = cl_fd;
		if (epoll_ctl(s.get_epollFD(), EPOLL_CTL_ADD, cl_fd, &ev) < 0)
		{
			std::cout << "epoll_ctl: client fd error" << std::endl;
			close(cl_fd);
			continue;
		}
		s.addClientMap(cl_fd);
		std::cout << C_Y "New client connected: fd " C_RESET << cl_fd << std::endl;
	}
}

//Need to investigate... Basically want to know if this is the correct way
void handleClientRead(Server &s, int fd)
{
	char buffer[BUFFER_SIZE + 1];
	while (true)
	{
		ssize_t count = read(fd, buffer, BUFFER_SIZE);
		if (count == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			std::cout << "read error on fd: " << fd << std::endl;
			cleanupClient(s, fd);
			break;
		}
		else if (count == 0)
		{
			std::cout << C_R "Client disconnected: fd " C_RESET << fd << std::endl;
			cleanupClient(s, fd);
			break;
		}
		else
		{
			buffer[count] = '\0';
			std::cout << "Received from " << fd << ": " << buffer;

			Parser parser;
			Command cmd = parser.parse(buffer);	// trying to understand how to execute command :(
			cmd.printCommand();

			// Store response in a write buffer associated with the client and enable EPOLLOUT only when needed
			//s.queueWrite(fd, std::string(buffer, count));	//write on a buffer for this client (known by its fd) need to check how u handle buffers

			struct epoll_event ev;
			ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
			ev.data.fd = fd;
			epoll_ctl(s.get_epollFD(), EPOLL_CTL_MOD, fd, &ev);
		}
	}
}

void handleClientSend(Server &s, std::string &msg, int fd)
{
	
	ssize_t sent = send(fd, msg.c_str(), msg.size(), 0); // this function will be called from server channels to send to all the clients the buffer specified.
	if (sent == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return; // try again later
		std::cout << "send error" << std::endl;
		cleanupClient(s, fd);
		return;
	}
	msg.erase(0, sent);
	if (msg.empty())
	{
		// Disable EPOLLOUT
		struct epoll_event ev;
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = fd;
		epoll_ctl(s.get_epollFD(), EPOLL_CTL_MOD, fd, &ev);
	}
}






int main(int ac, char **av)
{
	int sv_fd, epoll_fd;
	try
	{
		if (ac != 3)
			throw std::string("Wrong arguments");
		sv_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (sv_fd < 0)
		{
			std::cout << "socket error" << std::endl;
			return (-1);
		}
		if (valid_port(av[1]) == false)
		{
			std::cout << "Invalid port" << std::endl;
			return (-1);
		}
		
		setNonBlocking(sv_fd); //set fcntl to non blocking
		Server s(sv_fd, atoi(av[1]), av[2]);

		s.set_server_name("IRC_Server....");

		struct sockaddr_in server_addr;
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET; // set IPv4 family
		server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
		server_addr.sin_port = htons(s.get_port()); // convert port to network byte order

		//Binding: 
		if (bind(s.get_serverFD(), (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		{
			std::cout << "bind error" << std::endl;
			close(s.get_serverFD());
			return (-1);
		}

		//Listen:
		if(listen(s.get_serverFD(), HOLD_NON_ACCEPTED) < 0)
		{
			std::cout << "listen error" << std::endl;
			close(s.get_serverFD());
			return (-1);
		}

		//Creating epoll instance
		epoll_fd = epoll_create1(0);
		if (epoll_fd < 0)
		{
			std::cout << "epoll_create1 error" << std::endl;
			close(s.get_serverFD());
			return (-1);
		}

		s.set_epollFD(epoll_fd);
		struct epoll_event ev;
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = s.get_serverFD();
			
		if (epoll_ctl(s.get_epollFD(), EPOLL_CTL_ADD, s.get_serverFD(), &ev) < 0)
		{
			std::cout << "epoll_ctl: server_fd error" << std::endl;
			close(s.get_serverFD());
			return (-1);
		}

		std::cout << "Server started on port " << C_R << s.get_port() << C_RESET << std::endl;
		std::cout << "Server started on pass " << C_R << av[2] << C_RESET << std::endl;

		struct epoll_event events[MAX_EVENTS];

		//Event loop
		while (true)
		{
			int num_ready = epoll_wait(s.get_epollFD(), events, MAX_EVENTS, -1);
			if (num_ready < 0)
			{
				std::cerr << "epoll_wait error\n";
				break;
			}

			for (int i = 0; i < num_ready; ++i)
			{
				int fd = events[i].data.fd;
				uint32_t ev = events[i].events;
				if (fd == s.get_serverFD())
					handleNewConnection(s);
				else
				{
					if (ev & EPOLLIN)
						handleClientRead(s, fd);
					if (ev & EPOLLOUT)
					{
						std::string msg = "//just for compilation";
						handleClientSend(s, msg, fd);
					}
				}
			}
		}
		close(s.get_serverFD());
		close(s.get_epollFD());
		return (0);
	}
	catch(std::string &e)
	{
		std::cout << e << std::endl;
	}
}	

// https://www.suchprogramming.com/epoll-in-3-easy-steps/ 



//	EPOLLIN EPOLLOUT EPOLLET
//	https://chatgpt.com/share/67ff80a6-50e0-800a-9aa3-35fbffe54d04











				// int num_fd_ready = epoll_wait(s.get_epollFD(), events, MAX_EVENTS, -1);
				// if (num_fd_ready < 0)
				// {
				// 	std::cout << "epoll_wait error" << std::endl;
				// 	break ;
				// }
				// for (int i = 0; i < num_fd_ready; ++i)
				// {
				// 	int fd = events[i].data.fd;
				// 	if (fd == s.get_serverFD())
				// 	{



				// 	std::cout << "Accept new client here:" <<std::endl;
				// 		while (true)
				// 		{
				// 			int cl_fd = accept(s.get_serverFD(), NULL, NULL);
				// 			if (cl_fd < 0)
				// 			{
				// 				if (errno == EAGAIN || errno == EWOULDBLOCK)
				// 					break; // all connections accepted
				// 				std::cout << "accept error" << std::endl;
				// 				break;
				// 			}
				// 			setNonBlocking(cl_fd);
				// 			struct epoll_event ev_cl;
				// 			ev_cl.events = EPOLLIN | EPOLLET;
				// 			ev_cl.data.fd = cl_fd;
				// 			if (epoll_ctl(s.get_epollFD(), EPOLL_CTL_ADD, cl_fd, &ev_cl) < 0)
				// 			{
				// 				std::cout << "epoll_ctl: client fd error" << std::endl;
				// 				close(cl_fd);
				// 				continue;
				// 			}
				// 			s.addClientMap(cl_fd);
				// 			std::cout << C_Y "New client connected: fd " C_RESET << cl_fd << std::endl;
				// 		}
				// 		std::cout << "END Accept new client" <<std::endl;
				// 	}










				//NEED TO REDO PROPERLY!!
				// 	else
				// 	{
				// 		std::cout << "Read client here:" <<std::endl;
				// 		while (true)
				// 		{
				// 			//set server to listen EPOLLIN | EPOLLET
				// 			ssize_t count = read(fd, buffer, BUFFER_SIZE);
				// 			if (count == -1)
				// 			{
				// 				if (errno == EAGAIN || errno == EWOULDBLOCK)
				// 					break; // no more data
				// 				std::cout << "read error" << std::endl;
				// 				close(fd);
				// 				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
				// 				s.rmClientMap(fd);
				// 				break;
				// 			}
				// 			else if (count == 0)
				// 			{
				// 				std::cout << C_R "Client disconnected: fd " C_RESET << fd << std::endl;
				// 				s.rmClientMap(fd);
				// 				close(fd);
				// 				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
				// 				break;
				// 			}
				// 			else
				// 			{ // Not working as expected. Need to work properly to set epollin epollout epollet!!!
				// 				buffer[count] = '\0';
				// 				std::cout << "Received from " << fd << ": " << buffer;
				// 				Parser parser;
				// 				Command testInputsCmd = parser.parse(buffer);
				// 				testInputsCmd.printCommand();
				// 				ev.events = EPOLLOUT | EPOLLET;
				// 				epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
				// 				ssize_t sent = send(fd, buffer, count, 0);
				// 				if (sent == -1 && (errno != EAGAIN && errno != EWOULDBLOCK))
				// 				{
				// 					std::cout << "send error" << std::endl;
				// 					close(fd);
				// 					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
				// 					s.rmClientMap(fd);
				// 				}
				// 				ev.events = EPOLLOUT | EPOLLET;
				// 			}
				// 		}
				// 		std::cout << "END Read client" <<std::endl;
				
