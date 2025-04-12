/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/12 19:07:00 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Command.hpp"

int main() {
	Parser parser;
	Command cmd = parser.parse("PRIVMSG Bob Hello World");
	cmd.printCommand();
	
	std::cout << "\nTesting Command copy constructor:\n";
	Command cmdCopy(cmd); // Copy constructor
	cmdCopy.printCommand();

	std::cout << "\nTesting Command copy assignment:\n";
	Command anotherCmd;
	anotherCmd = cmd; // Copy assignment
	anotherCmd.printCommand();

	std::cout << "\nTesting Parser copy constructor:\n";
	Parser parserCopy(parser); // Copy constructor
	Command cmdFromCopy = parserCopy.parse("JOIN #channel");
	cmdFromCopy.printCommand();

	std::cout << "\nTesting Parser copy assignment:\n";
	Parser anotherParser;
	anotherParser = parser; // Copy assignment
	Command cmdFromAssigned = anotherParser.parse("PING server");
	cmdFromAssigned.printCommand();
	return 0;
}
