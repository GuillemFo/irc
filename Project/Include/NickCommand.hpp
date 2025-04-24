/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 07:59:32 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/24 12:17:29 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICKCOMMAND_HPP
# define NICKCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class NickCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		NickCommand();
		NickCommand(const NickCommand& src);
		NickCommand& operator=(const NickCommand& src);
		virtual ~NickCommand();

		NickCommand(Server* server);

		bool isValidNick(const std::string& name);
		virtual void execute(const Command& cmd, Client& sender);
};

#endif
