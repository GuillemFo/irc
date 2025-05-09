/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:08:06 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/09 09:49:06 by rzhdanov         ###   ########.fr       */
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
Bot::Bot(const std::string& server, int port, const std::string& password)
	: _socket_fd(-1), _connected(false) {
	_connected = connectToServer(server, port, password);
}

Bot::~Bot() {
	if (this->_connected) {
		close(this->_socket_fd);
	}
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

bool Bot::isConnected() const {
	return this->_connected;
}

void Bot::sendRaw(const std::string& msg) {
	std::string full = msg +"\r\n";
	send(this->_socket_fd, full.c_str(), full.size(), 0);
}

void Bot::handleMessage(const std::string& msg) {
	//TODO: probably use compare instead to work avoid getting ping from 
	// a random position in the message (if someone send a message 
	// containing PING)
	if (msg.find(PING) == 0) {
		sendRaw("PONG" + msg.substr(4));
		return ;
	}
	size_t exclamation = msg.find('!');
	size_t colon = msg.find(" :");
	if (colon != std::string::npos && exclamation != std::string::npos) {
		std::string sender = msg.substr(1, exclamation - 1);
		std::string content = msg.substr(colon + 2);
		size_t limit = this->_commands.size();
		for (size_t i = 0; i < limit; ++i) {
			if (this->_commands[i]->matches(content)) {
				sendRaw(this->_commands[i]->respond(sender));
				break;
			}
		}
	}
}

void Bot::run() {
	char buffer[BUFFER_SIZE];
	while (true) {
		std::memset(buffer, 0, BUFFER_SIZE);
		int bytes = recv(this->_socket_fd, buffer, BUFFER_SIZE - 1, 0);
		if (bytes <= 0) {
			break;
		}
		std::string msg(buffer);
		std::cout << msg;
		handleMessage(msg);
	}
}