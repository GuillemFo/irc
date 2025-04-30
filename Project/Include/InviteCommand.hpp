/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:22:45 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/29 20:29:53 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITECOMMAND_HPP
# define INVITECOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class InviteCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		InviteCommand();
		InviteCommand(const InviteCommand& src);
		InviteCommand& operator=(const InviteCommand& src);
		virtual ~InviteCommand();

		InviteCommand(Server* server);

		virtual void execute(const Command& cmd, Client& sender);
};

#endif
