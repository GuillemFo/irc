/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:22:45 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/29 20:29:53 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPICCOMMAND_HPP
# define TOPICCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class TopicCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		TopicCommand();
		TopicCommand(const TopicCommand& src);
		TopicCommand& operator=(const TopicCommand& src);
		virtual ~TopicCommand();

		TopicCommand(Server* server);

		virtual void execute(const Command& cmd, Client& sender);
};

#endif
