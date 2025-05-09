/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:39:54 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/09 11:30:21 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser () {}
Parser::Parser(const Parser& src) {
	(void) src;
}
Parser& Parser::operator=(const Parser& src) {
	if (this != &src) {
		//nothing to do here as there are no data members in a Parser object
	}
	return *this;
}
Parser::~Parser () {}

Command Parser::parse(const std::string& line) {
	Command cmd;
	std::istringstream iss(line);
	std::string word;
	if (!(iss >> word))
		return cmd;
	cmd.setName(word);

	bool trailing = false;
	std::string trailingArg;
	
	while (iss >> word) {
		if (!trailing && word[0] == ':') {
			trailing = true;
			trailingArg = word.substr(1);
			std::string rest;
			std::getline(iss, rest);
			trailingArg += rest;
			cmd.addArg(trailingArg);
		}
		else if(!trailing) {
			cmd.addArg(word);
		}
		else {
			std::cout << "You are not supposed to get here! Check " <<
			"the parsing logic!" << std::endl;
		}
	}
	return cmd;
}
