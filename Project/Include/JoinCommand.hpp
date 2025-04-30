/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 21:42:22 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/30 13:48:53 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class JoinCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		JoinCommand();
		JoinCommand(const JoinCommand& src);
		JoinCommand& operator=(const JoinCommand& src);
		virtual ~JoinCommand();

		JoinCommand(Server* server);

		static bool isValidChannelName(const std::string& name);
		virtual void execute(const Command& cmd, Client& sender);
};

#endif
