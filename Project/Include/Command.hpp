/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:56:38 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/13 13:42:27 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <iostream>
# include <vector>

class Command {
	private:
		std::string _name;
		std::vector<std::string> _args;

	public:
		Command();
		Command(const Command& src);
		Command& operator=(const Command& src);
		~Command();
		void setName(const std::string& name);
		void addArg(const std::string& arg);
		const std::string& getName() const;
		const std::vector<std::string>& getArgs() const;
		void printCommand() const;
};

#endif
