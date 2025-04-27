/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeTests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:35:10 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/27 23:22:30 by rzhdanov         ###   ########.fr       */
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

void test_mode_command_basic() {
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	ASSERT_TRUE(true, "ModeCommand constructed with server pointer");
}

void test_mode_command_no_args() {
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 1);

	Command cmd;
	cmd.setName("MODE");

	modeCmd.execute(cmd, client);

	ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after MODE with no args");
	std::cout << "===outBuffer content:\n" << client.getNextOutBufferChunk();
}

void test_mode_command_invalid_target() {
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 2);

	Command cmd;
	cmd.setName("MODE");
	cmd.addArg("invalidTarget"); // Not a channel

	modeCmd.execute(cmd, client);

	ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after MODE with invalid target");
	std::cout << "===outBuffer content:\n" << client.getNextOutBufferChunk();
}

void test_mode_command_nonexistent_channel() {
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 3);

	Command cmd;
	cmd.setName("MODE");
	cmd.addArg("#nonexistent");

	modeCmd.execute(cmd, client);

	ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after MODE with non-existent channel");
	std::cout << "===outBuffer content:\n" << client.getNextOutBufferChunk();
}

void test_mode_command_print_modes() {
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 4);

	const std::string ch_name = "#testchannel";
	server.addChannelMap(ch_name);
	Channel* channel = server.getChannel(ch_name);
	channel->addClient(&client);

	Command cmd;
	cmd.setName("MODE");
	cmd.addArg("#testchannel");

	modeCmd.execute(cmd, client);

	ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after MODE on existing channel (print modes)");
	std::cout << "===outBuffer content:\n" << client.getNextOutBufferChunk();
}

void test_mode_command_set_invite_only() {
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 5);
	const std::string ch_name = "#inviteonly";
	server.addChannelMap(ch_name);
	Channel* channel = server.getChannel(ch_name);
	channel->addClient(&client);
	channel->addOperator(&client); // Client becomes channel operator

	Command cmd;
	cmd.setName("MODE");
	cmd.addArg("#inviteonly");
	cmd.addArg("+i");

	modeCmd.execute(cmd, client);

	ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after setting +i mode");
	std::cout << "===outBuffer content:\n" << client.getNextOutBufferChunk();
}

void test_mode_command_set_topic_restricted() {
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 6);
	const std::string ch_name = "#topictest";
	// Channel channel(ch_name);
	server.addChannelMap(ch_name);
	Channel* channel = server.getChannel(ch_name);
	client.set_nick("John");
	channel->addClient(&client);
	channel->addOperator(&client);
	std::cout << client.get_nick() << " is operator: "
		<< channel->isOperator(client.get_nick()) << std::endl;
	
	Command cmd;
	cmd.setName("MODE");
	cmd.addArg("#topictest");
	cmd.addArg("+t");
	// cmd.printCommand();
	std::cout << "number of args: " << cmd.getArgs().size() << std::endl;
	modeCmd.execute(cmd, client);

	ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after setting +t mode");
	std::cout << "===outBuffer content:\n" << client.getNextOutBufferChunk();
}

int main() {
	std::cout << "Running ModeCommand tests..." << std::endl;
	test_mode_command_basic();
	test_mode_command_no_args();
	test_mode_command_invalid_target();
	test_mode_command_nonexistent_channel();
	test_mode_command_print_modes();
	test_mode_command_set_invite_only();
	test_mode_command_set_topic_restricted();
	return 0;
}
