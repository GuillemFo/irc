/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/13 22:57:01 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "PrivmsgCommand.hpp"
#include "Tools.hpp"
#include "Colors.hpp"
#include "JoinCommand.hpp"

int main() {
	Parser parser;
	std::cout << "\n\n**** TESTING: Parser ****" << std::endl;
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

	Client* bob = new Client(42);
	std::cout << "\n**** TESTING: PrivateMessage ****" << std::endl;
	PrivmsgCommand* PrivateMessage = new PrivmsgCommand;
	std::cout << "==== Test 1: No arguments ====" << std::endl;
	PrivateMessage->execute(parser.parse("PRIVMSG"), *bob);

	std::cout << "\n==== Test 2: One argument (target only) ====" << std::endl;
	PrivateMessage->execute(parser.parse("PRIVMSG Alice"), *bob);

	std::cout << "\n==== Test 3: Correct 2 args ====" << std::endl;
	PrivateMessage->execute(parser.parse("PRIVMSG Alice :Hello, Alice, how are you doing?"), *bob);

	std::cout << "\n==== Test 4: Multi-word message ====" << std::endl;
	PrivateMessage->execute(parser.parse("PRIVMSG Alice Hello, Alice, how are you doing?"), *bob);

	std::cout << "\n==== Test 5: Two single word args ====" << std::endl;
	PrivateMessage->execute(parser.parse("PRIVMSG Alice Hello"), *bob);

	JoinCommand join;
	std::cout << "\n**** TESTING: JOIN ****" << std::endl;
	std::cout << "==== JOIN isValidChannelName Tests ====" << std::endl;

	std::cout << "\nTest 1: Empty name" << std::endl;
	join.isValidChannelName("");

	std::cout << "\nTest 2: Name without # or &" << std::endl;
	join.isValidChannelName("general");

	std::cout << "\nTest 3: Name with space" << std::endl;
	join.isValidChannelName("#gen eral");

	std::cout << "\nTest 4: Name with comma" << std::endl;
	join.isValidChannelName("#general,42");

	std::cout << "\nTest 5: Name too long (>50)" << std::endl;
	join.isValidChannelName("#" + std::string(51, 'a'));

	std::cout << "\nTest 6: Valid name" << std::endl;
	join.isValidChannelName("#general");

	// --------------------------------------

	std::cout << "\n==== JOIN execute Tests ====" << std::endl;

	std::cout << "\nTest 7: No arguments" << std::endl;
	join.execute(parser.parse("JOIN"), *bob);

	std::cout << "\nTest 8: Invalid channel name (no #)" << std::endl;
	join.execute(parser.parse("JOIN general"), *bob);

	std::cout << "\nTest 9: Valid JOIN command" << std::endl;
	join.execute(parser.parse("JOIN #general"), *bob);

	delete bob;
	delete PrivateMessage;
	
	return 0;
}
