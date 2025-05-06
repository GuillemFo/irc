/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 19:50:40 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/06 19:51:04 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHOCOMMAND_HPP
# define WHOCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class WhoCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		WhoCommand();
		WhoCommand(const WhoCommand& src);
		WhoCommand& operator=(const WhoCommand& src);
		virtual ~WhoCommand();

		WhoCommand(Server* server);

		virtual void execute(const Command& cmd, Client& sender);
};

#endif
