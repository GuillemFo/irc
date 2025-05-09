/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:39:36 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/06 19:38:29 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDISPATCHER_HPP
# define COMMANDISPATCHER_HPP

# include "CommandHandler.hpp"
# include <map>
# include <string>

//class Client;
class Command;

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
