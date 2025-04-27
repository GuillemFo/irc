/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeTests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:35:10 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/27 09:40:38 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include "ModeCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) std::cerr << "FAIL: " << msg << std::endl; \
    else std::cout << "PASS: " << msg << std::endl; \
} while (0)

// Basic test: constructing ModeCommand
void test_mode_command_basic() {
    Server server(0, 6667, "pass");
    ModeCommand modeCmd(&server);
    ASSERT_TRUE(true, "ModeCommand constructed with server pointer");
}

// Test: MODE command with no args
void test_mode_command_no_args() {
    Server server(0, 6667, "pass");
    ModeCommand modeCmd(&server);
    Client client(&server, 1);

    Command cmd;
    cmd.setName("MODE");

    modeCmd.execute(cmd, client);

    ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after MODE with no args");
    std::cout << "===outBuffer content:" << std::endl;
    std::cout << client.getNextOutBufferChunk();
}

// Test: MODE command with invalid target (not a channel)
void test_mode_command_invalid_target() {
    Server server(0, 6667, "pass");
    ModeCommand modeCmd(&server);
    Client client(&server, 2);

    Command cmd;
    cmd.setName("MODE");
    cmd.addArg("invalidTarget"); // Not starting with '#'

    modeCmd.execute(cmd, client);

    ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after MODE with invalid target");
    std::cout << "===outBuffer content:" << std::endl;
    std::cout << client.getNextOutBufferChunk();
}

// Test: MODE command with non-existent channel
void test_mode_command_nonexistent_channel() {
    Server server(0, 6667, "pass");
    ModeCommand modeCmd(&server);
    Client client(&server, 3);

    Command cmd;
    cmd.setName("MODE");
    cmd.addArg("#nonexistent");

    modeCmd.execute(cmd, client);

    ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after MODE with non-existent channel");
    std::cout << "===outBuffer content:" << std::endl;
    std::cout << client.getNextOutBufferChunk();
}

// Test: MODE command with existing channel but no extra args (print modes)
void test_mode_command_print_modes() {
    Server server(0, 6667, "pass");
    ModeCommand modeCmd(&server);
    Client client(&server, 4);

    Channel channel("#testchannel");
    channel.addClient(&client); // Assume channel has addClient(Client*)
    server.addChannelMap(channel.get_name()); // Server should have addChannel(Channel*)

    Command cmd;
    cmd.setName("MODE");
    cmd.addArg("#testchannel");

    modeCmd.execute(cmd, client);

    ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after MODE on existing channel (print modes)");
    std::cout << "===outBuffer content:" << std::endl;
    std::cout << client.getNextOutBufferChunk();
}

int main() {
    std::cout << "Running ModeCommand tests..." << std::endl;
    test_mode_command_basic();
    test_mode_command_no_args();
    test_mode_command_invalid_target();
    test_mode_command_nonexistent_channel();
    test_mode_command_print_modes();
    return 0;
}
