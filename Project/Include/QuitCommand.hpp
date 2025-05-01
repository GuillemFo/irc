/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:40:49 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/28 10:03:47 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUITCOMMAND_HPP
# define QUITCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class QuitCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		QuitCommand();
		QuitCommand(const QuitCommand& src);
		QuitCommand& operator=(const QuitCommand& src);
		virtual ~QuitCommand();

		QuitCommand(Server* server);
		Server				*getServer() const;

		
		virtual void execute(const Command& cmd, Client& sender);
};

#endif