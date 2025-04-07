/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/07 13:31:19 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


/*
Your executable will be run as follows:
./ircserv <port> <password>
*/

/*
Not operational 07/04/25 10.11 need to redo .cpp
Remember init struct sockaddr_in sv_addr and for client too and same with socklen_t for sv and client addr_lenght
Client channel and server are now updated to accept multiple clients. Need to change main to use the new functions.




*/



int main(int ac, char **av)
{
	try
	{
		if (ac != 3)
			throw std::string("Wrong arguments");
	//////////////// TESTING //////////////
		int server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd < 0)
		{
			std::perror("socket");
			return (-1);
		}
		Server s(server_fd, av[1], atoi(av[2]));
		if (valid_port(av[1]) == false)
		{
			std::perror("Invalid port");
			return (-1);
		}

		s.set_server_name("IRC Server....");



	// move to client!! ??
	s.client_addr_len = sizeof(s.client_addr);
	char	buffer[1024];
	

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // set IPv4 family
	server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
	server_addr.sin_port = htons(s.get_port()); // convert port to network byte order // should set the incoming port?

	//Binding:
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		std::perror("bind");
		close(s.get_serverFD());
		return (-1);
	}

	//Listen:
	if(listen(s.get_serverFD(), 5) < 0)
	{
		std::perror("listen");
		close(s.get_serverFD());
		return (-1);
	}

	std::cout << "Server Listening on port:" << s.get_port() << std::endl;


	// REDO this with epoll() !!!!! 07/04/25 13.31
	// //Accept conn
	// struct sockaddr_in client_addr;
	// socklen_t client_addr_len;
	// int client_fd = accept(s.get_serverFD(), (struct sockaddr *)&client_addr, &client_addr_len);
	// if (client_fd < 0)
	// {
	// 	std::perror("accept");
	// 	close(s.get_serverFD());
	// 	return (-1);
	// }
	// s.addClientMap(client_fd);

	// std::cout << "Client connected" <<std::endl;

	// //Read data from cl
	// s.set_auth(false);	//pending to move to client
	// s.set_reg(false);	//pending to move to client
	// while (1) //infinite loop
	// {
	// 	ssize_t bytes_read = read( , buffer, sizeof(buffer) -1);
	// 	if (bytes_read < 0)
	// 	{
	// 		std::perror("read");
	// 		close(s.client_fd);
	// 		close(s.get_serverFD());
	// 		return (-1);
	// 	}

	// 	buffer[bytes_read] = '\0'; // hard null end buffer
	// 	s.buff_to_string(buffer);
	// 	//std::cout << "Received message: " << buffer <<std::endl;
	// }

	// close(s.client_fd);
	close(s.get_serverFD());
	return (0);
	}
	catch(std::string &e)
	{
		std::cout << e << std::endl;
	}
	
}


// Need to prepare epoll, create the user with the incoming fd and store its data.
