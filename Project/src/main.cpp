/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/08 15:10:15 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


/*
Your executable will be run as follows:
./ircserv <port> <password>
*/

//Remember init struct sockaddr_in sv_addr and for client too and same with socklen_t for sv and client addr_lenght



void	setNonBlocking(int sv_fd)
{
	int flag = fcntl(sv_fd, F_GETFL, 0);
	if (flag == -1)
	{
		std::perror("fcntl F_GETFL");
		std::exit(-1);
	}
	if (fcntl(sv_fd, F_SETFL, flag | O_NONBLOCK == -1))
	{
		std::perror("fcntl F_SETFL");
		std::exit(-1);
	}
}



int main(int ac, char **av)
{
	//Needed to work
	int sv_fd, epoll_fd;
    struct sockaddr_in server_addr;

	try
	{
		if (ac != 3)
			throw std::string("Wrong arguments");
	//////////////// TESTING //////////////
		sv_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (sv_fd < 0)
		{
			std::perror("socket");
			return (-1);
		}
		if (valid_port(av[1]) == false)
		{
			std::perror("Invalid port");
			return (-1);
		}
		setNonBlocking(sv_fd); //set fcntl to non blocking
		Server s(sv_fd, av[1], atoi(av[2]));

		s.set_server_name("IRC Server....");

	///////////////// Until here all ok /////////////// 07/04/25 13.53

	
	
	

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // set IPv4 family
	server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
	server_addr.sin_port = htons(s.get_port()); // convert port to network byte order

	//Binding: 
	if (bind(s.get_serverFD(), (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		std::perror("bind");
		close(s.get_serverFD());
		return (-1);
	}

	//Listen:
	if(listen(s.get_serverFD(), HOLD_NON_ACCEPTED) < 0)
	{
		std::perror("listen");
		close(s.get_serverFD());
		return (-1);
	}

	//Creating epoll instance
	epoll_fd = epoll_create1(0);
	if (epoll_fd < 0)
	{
		std::perror("epoll_create1");
		close(s.get_serverFD());
		return (-1);
	}
	s.set_epollFD(epoll_fd);
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = s.get_serverFD();
	if (epoll_ctl(s.get_epollFD(), EPOLL_CTL_ADD, s.get_serverFD(), &ev) < 0)
	{
		std::perror("epoll_ctl: server_fd");
		close(s.get_serverFD());
		return (-1);
	}

	std::cout << "Server started on port " << C_R << s.get_port() << C_RESET << std::endl;

	/*
		MAX_EVENTS → how many FDs epoll_wait will return at once (not max clients).
		BUFFER_SIZE → how many bytes you read from a socket at once.
	*/
	#define MAX_EVENTS 64
	#define BUFFER_SIZE 1024
	struct epoll_event events[MAX_EVENTS];
	char buffer[BUFFER_SIZE];

	//Event loop
	while (true)
	{
		int num_fd_ready = epoll_wait(s.get_epollFD(), events, MAX_EVENTS, -1);
		if (num_fd_ready < 0)
		{
			std::perror("epoll_wait");
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
					std::perror("accept");
					continue ;
				}
				s.addClientMap(cl_fd);
				setNonBlocking(cl_fd);	//This should go to server class, look on the map and pul the client fd.
				
				//Reg cl socket:
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = cl_fd; //This should go to server class, look on the map and pul the client fd.
			}
		}
	}







	close(s.get_serverFD());
	return (0);
	}
	catch(std::string &e)
	{
		std::cout << e << std::endl;
	}
	
}


// Need to prepare epoll, create the user with the incoming fd and store its data.
