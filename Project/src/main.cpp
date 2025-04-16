/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/17 00:07:29 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Tools.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "PrivmsgCommand.hpp"
#include "Tools.hpp"
#include "Colors.hpp"
#include "JoinCommand.hpp"
#include "Parser.hpp"
#include "Command.hpp"
#include "CommandDispatcher.hpp"
#include <iostream>

// Simple mock CommandHandler
class MockHandler : public CommandHandler {
private:
    std::string _name;

public:
    MockHandler(const std::string& name) : _name(name) {}

    virtual void execute(const Command& cmd, Client& sender) {
		(void) sender;
        std::cout << "[Handler Executed] " << _name << std::endl;
        std::cout << "Command: " << cmd.getName() << std::endl;
        std::vector<std::string> args = cmd.getArgs();
        for (size_t i = 0; i < args.size(); ++i) {
            std::cout << "Arg[" << i << "]: " << args[i] << std::endl;
        }
    }
};

// Dummy client just to pass something into execute()
class DummyClient : public Client {
public:
    DummyClient(int fd) : Client(fd) {}
    void sendError(const std::string& msg) {
        std::cout << "[Client Error] " << msg << std::endl;
    }
};

int main() {
    CommandDispatcher dispatcher;

    // Create mock handlers
    MockHandler* join = new MockHandler("JOIN");
    MockHandler* privmsg = new MockHandler("PRIVMSG");

    // Register mock handlers
    dispatcher.registerHandler("JOIN", join);
    dispatcher.registerHandler("PRIVMSG", privmsg);

    // Prepare dummy client
    DummyClient dummy(42);

    // Test 1: Known command JOIN
    Command joinCmd;
    joinCmd.setName("JOIN");
    joinCmd.addArg("#channel1");

    std::cout << "==== Test 1: JOIN ====" << std::endl;
    dispatcher.dispatch(joinCmd, dummy);

    // Test 2: Known command PRIVMSG with two args
    Command msgCmd;
    msgCmd.setName("PRIVMSG");
    msgCmd.addArg("Bob");
    msgCmd.addArg("Hello Bob!");

    std::cout << "\n==== Test 2: PRIVMSG ====" << std::endl;
    dispatcher.dispatch(msgCmd, dummy);

    // Test 3: Unknown command
    Command unknownCmd;
    unknownCmd.setName("KICK");

    std::cout << "\n==== Test 3: Unknown command ====" << std::endl;
    dispatcher.dispatch(unknownCmd, dummy);

    return 0;
}
