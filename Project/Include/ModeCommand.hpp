/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:13:57 by romanzdanov       #+#    #+#             */
/*   Updated: 2025/05/07 01:39:02 by rzhdanov         ###   ########.fr       */
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
		virtual void execute(const Command& cmd, Client& sender);
		std::string formatModeChangeMessage(const Client& sender,
			const Channel& channel,
			const std::string& modeChanges,
			const std::string& modeParameters);
};

#endif
