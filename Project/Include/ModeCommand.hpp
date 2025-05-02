/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:13:57 by romanzdanov       #+#    #+#             */
/*   Updated: 2025/05/02 22:21:53 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODECOMMAND_HPP
# define MODECOMMAND_HPP
# include "CommandHandler.hpp"
# include "Server.hpp"
# include <sstream>

class ModeCommand : public CommandHandler
{
	private:
		Server* _server;
		void handleChannelMode(Client& sender, Channel& channel,
			const Command& cmd);

	public:
		ModeCommand();
		ModeCommand(Server* server);
		ModeCommand(const ModeCommand& src);
		ModeCommand& operator=(const ModeCommand& src);
		virtual	~ModeCommand();
		//TODO: move int_to_string  to tools 
		std::string int_to_string(int value);
		virtual void execute(const Command& cmd, Client& sender);
};

#endif
