/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 07:59:32 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/16 07:59:59 by gforns-s         ###   ########.fr       */
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