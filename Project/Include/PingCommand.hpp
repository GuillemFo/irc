/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:28:11 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/25 09:28:55 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class PingCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		PingCommand();
		PingCommand(const PingCommand& src);
		PingCommand& operator=(const PingCommand& src);
		virtual ~PingCommand();

		PingCommand(Server* server);

		virtual void execute(const Command& cmd, Client& sender);
};

#endif