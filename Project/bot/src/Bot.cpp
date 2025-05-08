/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:08:06 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/09 00:28:49 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstdlib>
#include "Bot.hpp"
#include "BotPingCommand.hpp"

#define BUFFER_SIZE 1024
Bot::Bot() {};
Bot::Bot(const std::string& server, int port, const std::string& password) {
	connectToServer(server, port, password);
}
Bot::~Bot() {
	close(this->_socket_fd);
	for (size_t i = 0; i < this->_commands.size(); i++) {
		delete this->_commands[i];
	}
}

bool Bot::connectToServer(const std::string& server, int port,
						const std::string& password) {
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd < 0) {
		std::cerr << "Socket creation failed" << std::endl;
		return false;
	}
	struct sockaddr_in serv_addr;
	std::memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	inet_pton(AF_INET, server.c_str(), &serv_addr.sin_addr);
	if (connect(this->_socket_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)) < 0) {
		std::cerr << "Connection failed" << std::endl;
		return false;
	}
	if (!password.empty()) {
		sendRaw("PASS " + password);
	}
	sendRaw("NICK bot42");
	sendRaw("USER bot42 0 * :IRC Bot");
	sendRaw("JOIN #bot");

	return true;
}
