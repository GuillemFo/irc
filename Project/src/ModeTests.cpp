/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeTests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romanzdanov <romanzdanov@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:35:10 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/26 23:10:30 by romanzdanov      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Channel.hpp"
#include <iostream>
#include <vector>

int main() {
    // Create a dummy server
    Server server;

    // Create a dummy client (sender)
    Client sender;
    sender.set_nick("testuser"); // Assume you have a set_nick() method
    sender.set_clientFD(5); // Assume you have a set_clientFD() method

    // Create a dummy channel and register it on the server
    Channel channel("#testchannel");
    channel.addClient(&sender); // Assume channel has addClient(Client*)

    server.addChannel(&channel); // Assume server has addChannel(Channel*)

    // Create a ModeCommand handler
    ModeCommand modeCmd(&server);

    // Create a dummy MODE command: MODE #testchannel
    Command modeCommand;
    modeCommand.setName("MODE");
    modeCommand.addArg("#testchannel");

    // Execute the ModeCommand
    std::cout << "--- Testing MODE Command ---" << std::endl;
    modeCmd.execute(modeCommand, sender);

    // Print what was added to the sender's out buffer
    std::cout << "--- Outgoing messages for sender ---" << std::endl;
    std::cout << sender.getOutBuffer() << std::endl; // Assume getOutBuffer() returns std::string

    return 0;
}