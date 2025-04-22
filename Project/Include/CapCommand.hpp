/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:22:45 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/22 19:23:16 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAPCOMMAND_HPP
# define CAPCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class CapCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		CapCommand();
		CapCommand(const CapCommand& src);
		CapCommand& operator=(const CapCommand& src);
		virtual ~CapCommand();

		CapCommand(Server* server);

		virtual void execute(const Command& cmd, Client& sender);
};

#endif