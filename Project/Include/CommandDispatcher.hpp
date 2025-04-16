/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:39:36 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/17 00:59:19 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDISPATCHER_HPP
# define COMMANDISPATCHER_HPP

# include "Parser.hpp"
# include "Command.hpp"
# include "CommandHandler.hpp"
# include "Client.hpp"
# include <map>
# include <string>

class CommandHandler;
class CommandDispatcher {
	private:
		std::map<std::string, CommandHandler*> _handlers;
	public:
		CommandDispatcher();
		CommandDispatcher(const CommandDispatcher& src);
		CommandDispatcher& operator=(const CommandDispatcher& src);
		~CommandDispatcher();

		void registerHandler(const std::string& name, CommandHandler* handler);
		void dispatch(const Command& cmd, Client& sender);
};

#endif
