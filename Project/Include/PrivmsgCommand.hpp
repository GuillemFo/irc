/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:49:53 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/24 17:31:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSGCOMMAND_HPP
#define PRIVMSGCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class PrivmsgCommand : public CommandHandler {
	private:
		Server* _server;

	public:
		PrivmsgCommand();
		PrivmsgCommand(const PrivmsgCommand& src);
		PrivmsgCommand& operator=(const PrivmsgCommand& src);
		virtual ~PrivmsgCommand();

		PrivmsgCommand(Server* server);
		Server				*getServer() const;
		virtual void execute(const Command& cmd, Client& sender);
};

#endif
