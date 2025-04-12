/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:39:54 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/12 18:40:39 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <sstream>

Parser::Parser () {}

Command Parser::parse(const std::string& line) {
	Command cmd;
	std::istringstream iss(line);
	std::string word;
	
	if (iss >> word) {
		cmd.setName(word);
		while (iss >> word) {
			cmd.addArg(word);
		}
	}

	
	return cmd;
}
