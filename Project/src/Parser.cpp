/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:39:54 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/16 21:39:39 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser () {}
Parser::Parser(const Parser& src) {
	//just to maintain canonical form; parser will be called ad hoc and will
	//not have any data members. cleaner this way
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
	// uncomment for raw input testing
	// if (word[0] == '/')
	// 	word = word.substr(1);
	
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
