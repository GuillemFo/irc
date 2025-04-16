/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:39:54 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/16 14:17:34 by gforns-s         ###   ########.fr       */
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

	
	bool trailing = false;
	std::string trailingArg;
	
	std::cout << "here1: " << word << " line: " << line << std::endl; //command gets lost here!!! 
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
			std::cout << "here2: " << word << " line: " << line << std::endl; 
		}
		else {
			std::cout << "You are not supposed to get here! Check " <<
			"the parsing logic!" << std::endl;
		}
	}
	return cmd;
}
