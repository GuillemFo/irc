/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:08:00 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/09 00:24:22 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BOT_HPP
# define BOT_HPP

# include <vector>
# include <string>
# include "IncomingCommand.hpp"

class Bot {
	private:
		int _socket_fd;
		std::vector<IncomingCommand*> _commands;
		bool _connected;

		void sendRaw(const std::string& msg);
		void handleMessage(const std::string& msg);
		void setPassword(const std::string& password);
		bool connectToServer(const std::string& server, int port,
			const std::string& password);

	public:
		Bot();
		Bot(const std::string& server, int port, const std::string& password);
		~Bot();
		void run();
		bool isConnected() const;
};

#endif
