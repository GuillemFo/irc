/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:08:06 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/24 20:27:14 by rzhdanov         ###   ########.fr       */
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
	if (_connected) {
		this->_commands.push_back(new BotPingCommand());
	}
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
	// sendRaw("PRIVMSG rzhdanov :hello");

	return true;
}

bool Bot::isConnected() const {
	return this->_connected;
}

void Bot::sendRaw(const std::string& msg) {
	std::string full = msg +"\r\n";
	send(this->_socket_fd, full.c_str(), full.size(), 0);
	// std::cout << "[DEBUG]: " << "we are in sendRaw" << std::endl
				// << full << std::endl;
}

void Bot::handleMessage(const std::string& msg) {
	// std::cout << "[DEBUG] handleMessage() called with:\n" << msg << std::endl;
	//TODO: probably use compare instead to work avoid getting ping from 
	// a random position in the message (if someone send a message 
	// containing PING)
	// if (msg.find(PING) == 0) {
	// if (msg.compare(0, 4, PING) == 0) {
	size_t colon = msg.find(" :");
	std::string prefix = msg.substr(0, colon);
	size_t exclamation = prefix.find('!');
	// size_t colon = msg.find(':', 1);
	// std::cout << "[DEBUG] " << "OUR MESSAGE AT THE BEGINNING OF HANDLE MESSAGE: "
		// std::cout 
		// << std::endl << msg << std::endl; 
	// if (colon != std::string::npos && exclamation != std::string::npos) {
	if (colon != std::string::npos) {
		size_t space = msg.find(' ', 1);
		std::string sender;
		if (exclamation != std::string::npos) {
			sender = msg.substr(1, exclamation - 1);
		}
		else if(space != std::string::npos) {
			sender = msg.substr(1, space - 1);
		}
		else {
			std::cerr << "the message is not formatted properly" << std::endl;
			return ;
		}
		// std::cout << "[DEBUG]. sender is: " << sender << std::endl;
		std::string content = msg.substr(colon + 1);
		// std::cout<< "DEBUG INSIDE IF LOOP. COLON IS NOT NPOS" << std::endl;
		// std::cout << "Extracted content: [" << content << "]" << std::endl;
		size_t privmsgPos = msg.find("PRIVMSG ");
		if (privmsgPos != std::string::npos) {
			// size_t limit = this->_commands.size();
			// std::cout << "[DEBUG]: limit is: " << limit << std::endl;
			size_t targetStart = privmsgPos + 8;
			size_t targetEnd = msg.find(' ', targetStart);
			std::string target = msg.substr(targetStart, 
											targetEnd - targetStart);
			std::string response;
			if (content.find(PING) != std::string::npos) {
				response = "PRIVMSG " + sender + " :CUSTOM PONG FROM BOT42";
				// std::cout << "WE HAVE FOUND PING" << std::endl;
				// return ;
			}
			else {
				response = "PRIVMSG " + sender + " " + content + " to you too, dear "
							+ sender + "!";
			}
			sendRaw(response);
			// for (size_t i = 0; i < limit; ++i) {
			// 	if (this->_commands[i]->matches(content)) {
			// 		std::string response = "PRIVMSG " + sender + " :" + content + " to you too!";
			// 			// + this->_commands[i]->respond(sender);
			// 		// std::cout << "TESTING: " << response << std::endl;
			// 		sendRaw(response);
			// 		sendRaw(this->_commands[i]->respond(sender));
			// 		break;
			// 	}
			// }
		}
	}
}

void Bot::run() {
	char buffer[BUFFER_SIZE];
	while (true) {
		std::memset(buffer, 0, BUFFER_SIZE);
		int bytes = recv(this->_socket_fd, buffer, BUFFER_SIZE - 1, 0);
		std::string raw(buffer);
		// std::cout << "[RECV] \n" << raw << "[END OF RCV]" << std::endl;
		if (bytes <= 0) {
			break;
		}
		std::string msg(buffer);
		std::stringstream ss(msg);
		std::string line;
		while (std::getline(ss, line)) {
			if (!line.empty() && line[line.size() - 1] != '\r')
			{
				line.erase(line.size() - 1);
			}
			// std::cout << msg;
			handleMessage(line);
		}
	}
}