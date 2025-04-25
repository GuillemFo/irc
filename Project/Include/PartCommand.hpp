/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:05:43 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/25 17:33:35 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTCOMMAND_HPP
# define PARTCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class PartCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		PartCommand();
		PartCommand(const PartCommand& src);
		PartCommand& operator=(const PartCommand& src);
		virtual ~PartCommand();

		PartCommand(Server* server);

		virtual void execute(const Command& cmd, Client& sender);
};

#endif
