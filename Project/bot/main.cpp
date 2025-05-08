/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:20:50 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/09 00:15:08 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <cstdlib>
#include "Bot.hpp"

int main(int argc, char **argv) {
	//TODO: make the bot
	if (argc < 3 || argc > 4) {
		std::cerr << "Usage: " << argv[0] << " <server_ip> <port> [<pass>]"
			<< std::endl;
		return 1;
	}
	std::string server = argv[1];
	int port = std::atoi(argv[2]);
	std::string password;
	if (argc == 4) {
		password = argv[3];
	}
	Bot bot(server, port, password);
	bot.run();
	//we will need the Server to somehow add bot to every channel
	return 0;
}
