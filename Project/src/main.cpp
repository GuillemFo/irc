/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/12 20:02:47 by rzhdanov         ###   ########.fr       */
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

	std::vector<std::string> testInputs;

	testInputs.push_back("NICK Bob");
	testInputs.push_back("USER bob 0 * :Bob the builder");
	testInputs.push_back("JOIN #42");
	testInputs.push_back("PRIVMSG Alice :Hello Alice, how are you?");
	testInputs.push_back("NOTICE * :*** Looking up your hostname...");
	testInputs.push_back("QUIT :Client exited");
	testInputs.push_back("PING :irc.example.com");

	for (size_t i = 0; i < testInputs.size(); ++i) {
		std::cout << "=== Test Case " << i << " ===" << std::endl;
		std::cout << "Input: " << testInputs[i] << std::endl;
		Command testInputsCmd = parser.parse(testInputs[i]);
		testInputsCmd.printCommand();
		std::cout << std::endl;
	}

	return 0;
}
