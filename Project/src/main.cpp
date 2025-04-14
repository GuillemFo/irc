/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/14 14:20:53 by gforns-s         ###   ########.fr       */
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
/*
Your executable will be run as follows:
./ircserv <port> <password>
*/


//https://www.suchprogramming.com/epoll-in-3-easy-steps/

void	setNonBlocking(int sv_fd)
{
	int flag = fcntl(sv_fd, F_GETFL, 0);
	if (flag == -1)
	{
		std::cout << "fcntl F_GETFL" << std::endl;
		std::exit(-1);
	}
	if (fcntl(sv_fd, F_SETFL, flag | (O_NONBLOCK == -1)))
	{
		std::cout << "fcntl F_SETFL" << std::endl;
		std::exit(-1);
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
			std::cout << "socket" << std::endl;
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
			std::cout << "bind" << std::endl;
			close(s.get_serverFD());
			return (-1);
		}

		//Listen:
		if(listen(s.get_serverFD(), HOLD_NON_ACCEPTED) < 0)
		{
			std::cout << "listen" << std::endl;
			close(s.get_serverFD());
			return (-1);
		}

		//Creating epoll instance
		epoll_fd = epoll_create1(0);
		if (epoll_fd < 0)
		{
			std::cout << "epoll_create1" << std::endl;
			close(s.get_serverFD());
			return (-1);
		}

		s.set_epollFD(epoll_fd);
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = s.get_serverFD();
		
		if (epoll_ctl(s.get_epollFD(), EPOLL_CTL_ADD, s.get_serverFD(), &ev) < 0)
		{
			std::cout << "epoll_ctl: server_fd" << std::endl;
			close(s.get_serverFD());
			return (-1);
		}

		std::cout << "Server started on port " << C_R << s.get_port() << C_RESET << std::endl;
		std::cout << "Server started on pass " << C_R << av[2] << C_RESET << std::endl;

		/*
			MAX_EVENTS → how many FDs epoll_wait will return at once (not max clients).
			BUFFER_SIZE → how many bytes you read from a socket at once.
		*/
		#define MAX_EVENTS 64
		#define BUFFER_SIZE 1024
		struct epoll_event events[MAX_EVENTS];
		char buffer[BUFFER_SIZE];

		// Create a welcome channel to test.
	
		/*
		Page 25 modern-ircdocs ... pdf

		001 NICK :Welcome to the Internet Relay Network NICK!user@host
		002 NICK :Your host is ...
		003 NICK :This server was created ...
		004 NICK :server version info
		(More optional MOTD or notices...)
		375 NICK :- server Message of the Day -
		372 NICK :- Welcome to this IRC server!
		376 NICK :End of /MOTD command.
		*/

		//Event loop
		while (true)
		{
			int num_fd_ready = epoll_wait(s.get_epollFD(), events, MAX_EVENTS, -1);
			if (num_fd_ready < 0)
			{
				std::cout << "epoll_wait" << std::endl;
				break ;
			}
			for (int i = 0; i < num_fd_ready; ++i)
			{
				int fd = events[i].data.fd;
				if (fd == s.get_serverFD())
				{
				//Accept new clients here:
					int cl_fd = accept(s.get_serverFD(), NULL, NULL);
					if (cl_fd < 0)
					{
						std::cout << "accept" << std::endl;
						continue ;
					}
					setNonBlocking(cl_fd);
					//Reg cl socket:
					ev.events = EPOLLIN | EPOLLET;
					ev.data.fd = cl_fd;
					if (epoll_ctl(s.get_epollFD(), EPOLL_CTL_ADD, cl_fd, &ev) < 0)
					{
						std::cout << "epoll_ctl: client fd" << std::endl;
						close(cl_fd);
						continue ;
					}
					else
					{
						s.addClientMap(cl_fd);
						std::cout << C_Y "New client connected: fd " C_RESET << cl_fd << std::endl;
					}
				}
				else
				{
					// handle client data
					ssize_t count = read(fd, buffer, BUFFER_SIZE);
					if (count == -1)
					{
						std::cout << "read" << std::endl;
						close(fd);
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
					}
					else if (count == 0)	//client disconnecte?? what if client has nothing to send?
					{
						std::cout << C_R "Client disconnected: fd " C_RESET << fd << std::endl;
						s.rmClientMap(fd);
						close(fd);
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
					}
					else
					{
						buffer[count] = '\0';
						std::cout << "Received from " << fd << ": " << buffer;

						//Take buffer only when /r/n found.
						//Protect if pass not set, nick not set, name not set
						//create and allocate properly on each map with the incoming info
						//after all stored and valid client, parser buffer and look for commands.

						// Echo back
						// Implement commands?
						ssize_t sent = send(fd, buffer, count, 0);
						if (sent == -1)
						{
							std::cout << "send" << std::endl;
							close(fd);
							epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
						}
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
	return (0);
}

// https://www.suchprogramming.com/epoll-in-3-easy-steps/ 
