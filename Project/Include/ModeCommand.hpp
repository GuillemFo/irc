/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romanzdanov <romanzdanov@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:13:57 by romanzdanov       #+#    #+#             */
/*   Updated: 2025/04/26 21:10:01 by romanzdanov      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODECOMMAND_HPP
# define MODECOMMAND_HPP
# include "Command.hpp"
# include "Server.hpp"

class ModeCommand : public Command
{
	private:
		Server* _server;
		void handleChannelMode(Client& sender, Channel& channel, const Command& cmd);

	public:
		ModeCommand();
		ModeCommand(Server* server);
		ModeCommand(const ModeCommand& src);
		ModeCommand& operator=(const ModeCommand& src);
		virtual	~ModeCommand();

		virtual void execute(const Command& cmd, Client& sender);
}

#endif
