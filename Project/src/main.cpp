/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/05 16:17:31 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
Your executable will be run as follows:
./ircserv <port> <password>
*/


//https://www.suchprogramming.com/epoll-in-3-easy-steps/
/*
	MAX_EVENTS → how many FDs epoll_wait will return at once (not max clients).
	BUFFER_SIZE → how many bytes you read from a socket at once.
*/

// IMPORTANT!!!!!  every throw std::runtime_error will stop the server!!! remove immediately!! same with any throw
#define MAX_EVENTS 10 // 64
#define BUFFER_SIZE 512

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

	s.getClient(fd)->partAllChannels(); //need to rm from operator too.
	s.rmClientMap(fd);
	epoll_ctl(s.get_epollFD(), EPOLL_CTL_DEL, fd, NULL);
	close(fd);
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
		ev.events = EPOLLIN;
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


bool isValidIRCMessage(const std::string &input) {
	if (input.size() > 512)
		return false;
	return true;
}



void handleRead(Server &s, int fd)
{
	char buffer[BUFFER_SIZE];
	Client *client = s.getClient(fd);
	if (!client) {
		std::cout << "Invalid client fd: " << fd << std::endl; // probably change to std::cerr
		return;
	}
	while (true)
	{
		buffer[0] = '\0';
		ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
		if (bytes == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			std::cout << "read error on fd: " << fd << std::endl; // probably change to std::cerr
			cleanupClient(s, fd);
			break;
		}
		else if (bytes == 0)
		{
			std::cout << C_R "Client disconnected: fd " C_RESET << fd << std::endl;
			cleanupClient(s, fd);
			break;
		}
		else
		{
			std::string tmp(buffer, bytes);
			client->_in.append(tmp);
				if (!isValidIRCMessage(client->_in.getRaw()))
				{
					client->sendMessage(client->get_nick() + " :Input line was too long\r\n"); // is sent multiple times... 05.05.25 01.07 pm
					client->_in.clear();
					return ;
				}
			
			//std::cout << "Received from " << fd << ": " << replace_tool(replace_tool(buffer, "\r", "/r"), "\n", "/n\n"); buffer overflow when nc with massive text
			while (client->_in.hasCompleteCommand())
			{
				Parser parser;
				Command cmd = parser.parse(client->_in.extractCommand());
				Client *clientfd = s.getClient(fd);
				s._dispatcher.dispatch(cmd, *clientfd);
			}

			struct epoll_event ev;
			ev.events = EPOLLIN | EPOLLOUT;
			ev.data.fd = fd;
			epoll_ctl(s.get_epollFD(), EPOLL_CTL_MOD, fd, &ev);
		}
	}
}

void handleSend(Server &s, int fd)
{
	if (s.getClient(fd) == NULL)
	{
		return ;
	}
	if (s.getClient(fd)->_out.isEmpty())
		return ;
	std::string msg = s.getClient(fd)->_out.getMessage();
	ssize_t sent_bytes = send(fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);  // nosignal to protect from sending to a close socket
	if (sent_bytes == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return; // try again later, wait for epollout
		std::cout << "send error" << std::endl;
		cleanupClient(s, fd);
		return;
	}
	msg.erase(0, sent_bytes); // with the count of sent bytes we remove that number from the out buffer and continue
	s.getClient(fd)->_out.addOffset(sent_bytes);
	if (s.getClient(fd)->isOutEmpty())
	{
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = fd;
		epoll_ctl(s.get_epollFD(), EPOLL_CTL_MOD, fd, &ev);
		return ;
	}
}

void handle_sigint(int sig)
{
	if (sig)
		exit(0);
}

int main(int ac, char **av)
{
	int sv_fd, epoll_fd;
	try
	{
		signal(SIGINT, handle_sigint);
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
		
		setNonBlocking(sv_fd);
		Server s(sv_fd, atoi(av[1]), av[2]);
		
		s.registerAllCommands();
		s.set_server_name("127.0.0.1");
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
						handleRead(s, fd);
					if (ev & EPOLLOUT)
					{
						handleSend(s, fd);
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

