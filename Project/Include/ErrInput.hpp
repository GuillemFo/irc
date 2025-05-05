/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrInput.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:08:45 by codespace         #+#    #+#             */
/*   Updated: 2025/05/05 00:10:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRINPUT_HPP
# define ERRINPUT_HPP

# include "CommandHandler.hpp"
# include "Server.hpp"

class ErrInput : public CommandHandler {
	private:
		Server * _server;
	
	public:
		ErrInput();
		ErrInput(const ErrInput& src);
		ErrInput& operator=(const ErrInput& src);
		virtual ~ErrInput();

		ErrInput(Server* server);

		virtual void execute(const Command& cmd, Client& sender);
};

#endif