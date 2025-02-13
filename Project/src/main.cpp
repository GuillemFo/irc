/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/13 18:39:33 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


/*
Your executable will be run as follows:
./ircserv <port> <password>
*/
int main(int ac, char **av)
{
	Server Server;
	try
	{
		if (ac != 3)
			throw std::string("Wrong arguments");
		Server.check_port(av[1]);
		std::string in(av[2]);
		Server.set_pass(in);

	//////////////// TESTING //////////////

	int server_fd;
	int client_fd;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	char	buffer[1024];
	int port = 6667; //default irc port

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket");
		return (1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // set IPv4 family
	server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
	server_addr.sin_port = htons(port); // convert port to network byte order // should set the incoming port?

	//Binding:
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		close(server_fd);
		return (1);
	}

	//Listen:
	if(listen(server_fd, 5) < 0)
	{
		perror("listen");
		close(server_fd);
		return (1);
	}






	}
	catch(std::string &e)
	{
		std::cout << e << std::endl;
	}
	
}
