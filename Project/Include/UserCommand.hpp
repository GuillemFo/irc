/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 09:44:41 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/22 12:54:06 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERCOMMAND_HPP
# define USERCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class UserCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		UserCommand();
		UserCommand(const UserCommand& src);
		UserCommand& operator=(const UserCommand& src);
		virtual ~UserCommand();

		UserCommand(Server* server);

		bool isValidUser(const std::string& name);
		virtual void execute(const Command& cmd, Client& sender);
};

#endif
