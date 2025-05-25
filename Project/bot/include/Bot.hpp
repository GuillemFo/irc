/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:08:00 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/25 17:50:25 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BOT_HPP
# define BOT_HPP

# include <vector>
# include <string>
# include <csignal>
# include "IncomingCommand.hpp"

# define PING "PING"
# define PONG "PONG"

class Bot {
	private:
		int _socket_fd;
		std::vector<IncomingCommand*> _commands;
		bool _connected;
		static Bot* instance;
		static volatile sig_atomic_t stopRequested;

		void sendRaw(const std::string& msg);
		// void handleMessage(const std::string& msg);
		void setPassword(const std::string& password);
		bool connectToServer(const std::string& server, int port,
			const std::string& password);

	public:
		Bot();
		Bot(const std::string& server, int port, const std::string& password);
		~Bot();
		void run();
		bool isConnected() const;
		void handleMessage(const std::string& msg);
		static void handleSignal(int signal);
		void cleanup();
};

#endif
