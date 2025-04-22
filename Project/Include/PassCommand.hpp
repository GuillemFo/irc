/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 07:47:06 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/22 12:53:57 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASSCOMMAND_HPP
# define PASSCOMMAND_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class PassCommand : public CommandHandler {
	private:
		Server * _server;
	
	public:
		PassCommand();
		PassCommand(const PassCommand& src);
		PassCommand& operator=(const PassCommand& src);
		virtual ~PassCommand();

		PassCommand(Server* server);

		bool isValidPass(const std::string& name);
		virtual void execute(const Command& cmd, Client& sender);
};

#endif
