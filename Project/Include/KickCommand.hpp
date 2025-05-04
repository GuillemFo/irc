/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:40:49 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/04 21:53:12 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICKCOMMAND_HPP
# define KICKCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class KickCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		KickCommand();
		KickCommand(const KickCommand& src);
		KickCommand& operator=(const KickCommand& src);
		virtual ~KickCommand();

		KickCommand(Server* server);
		Server				*getServer() const;

		
		virtual void execute(const Command& cmd, Client& sender);
};

#endif