/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/17 21:13:22 by gforns-s         ###   ########.fr       */
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


	Server.client_addr_len = sizeof(Server.client_addr);
	char	buffer[1024];
	int port = Server.get_port();

	Server.server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (Server.server_fd < 0)
	{
		std::perror("socket");
		return (1);
	}

	memset(&Server.server_addr, 0, sizeof(Server.server_addr));
	Server.server_addr.sin_family = AF_INET; // set IPv4 family
	Server.server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
	Server.server_addr.sin_port = htons(port); // convert port to network byte order // should set the incoming port?

	//Binding:
	if (bind(Server.server_fd, (struct sockaddr *)&Server.server_addr, sizeof(Server.server_addr)) < 0)
	{
		std::perror("bind");
		close(Server.server_fd);
		return (1);
	}

	//Listen:
	if(listen(Server.server_fd, 5) < 0)
	{
		std::perror("listen");
		close(Server.server_fd);
		return (1);
	}

	std::cout << "Server Listening on port:" << port << std::endl;

	//Accept conn
	Server.client_fd = accept(Server.server_fd, (struct sockaddr *)&Server.client_addr, &Server.client_addr_len);
	if (Server.client_fd < 0)
	{
		std::perror("accept");
		close(Server.server_fd);
		return (1);
	}

	std::cout << "Client connected" <<std::endl;

	//Read data from cl
	Server.set_auth(false);
	Server.set_reg(false);
	while (1)
	{
		ssize_t bytes_read = read(Server.client_fd, buffer, sizeof(buffer) -1);
		if (bytes_read < 0)
		{
			std::perror("read");
			close(Server.client_fd);
			close(Server.server_fd);
			return (1);
		}

		buffer[bytes_read] = '\0'; // hard null end buffer
		Server.buff_to_string(buffer);
		//std::cout << "Received message: " << buffer <<std::endl;
	}

	close(Server.client_fd);
	close(Server.server_fd);
	return (0);
	}
	catch(std::string &e)
	{
		std::cout << e << std::endl;
	}
	
}
