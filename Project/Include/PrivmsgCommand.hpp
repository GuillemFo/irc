/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:49:53 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/13 21:30:11 by rzhdanov         ###   ########.fr       */
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
		virtual void execute(const Command& cmd, Client& sender);
};

#endif
