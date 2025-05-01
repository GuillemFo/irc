/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeTests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:35:10 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/01 22:20:47 by rzhdanov         ###   ########.fr       */
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

void printCallerFunction(const char* callerName) {
	std::cout << "///===***===///" << std::endl;
	std::cout << "Starting function: " << callerName << std::endl;
	std::cout << std::endl;
}

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
	client.set_nick("Ann");
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
	printCallerFunction(__FUNCTION__);
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 5);
	const std::string ch_name = "#inviteonly";
	server.addChannelMap(ch_name);
	Channel* channel = server.getChannel(ch_name);
	client.set_nick("Bob");
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
	printCallerFunction(__FUNCTION__);
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 6);
	const std::string ch_name = "#topictest";
	// Channel channel(ch_name);
	server.addChannelMap(ch_name);
	Channel* channel = server.getChannel(ch_name);
	client.set_nick("Casey");
	channel->addClient(&client);
	channel->addOperator(&client);
	// std::cout << client.get_nick() << " is operator: "
		// << channel->isOperator(client.get_nick()) << std::endl;
	
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

void test_mode_command_set_password_and_limit() {
	printCallerFunction(__FUNCTION__);
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 7);
	client.set_nick("Donny");
	const std::string ch_name = "#modetest1";
	server.addChannelMap(ch_name);
	Channel* channel = server.getChannel(ch_name);
	channel->addClient(&client);
	channel->addOperator(&client);

	Command cmd;
	cmd.setName("MODE");
	cmd.addArg(ch_name);
	cmd.addArg("+kl");
	cmd.addArg("secret123");
	cmd.addArg("42");

	modeCmd.execute(cmd, client);

	ASSERT_TRUE(channel->get_pass() == "secret123", "Channel password set correctly");
	ASSERT_TRUE(channel->get_userLimit() == 42, "Channel user limit set to 42");
}

void test_mode_command_password_and_remove_limit() {
	printCallerFunction(__FUNCTION__);
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 8);
	client.set_nick("Elle");
	const std::string ch_name = "#modetest2";
	server.addChannelMap(ch_name);
	Channel* channel = server.getChannel(ch_name);
	channel->addClient(&client);
	channel->addOperator(&client);
	channel->set_userLimit(50); // pre-set user limit

	Command cmd;
	cmd.setName("MODE");
	cmd.addArg(ch_name);
	cmd.addArg("+k");
	cmd.addArg("-l"); // password will be "-l"

	modeCmd.execute(cmd, client);

	ASSERT_TRUE(channel->get_pass() == "-l", "Password set to '-l'");
	ASSERT_TRUE(channel->get_userLimit() == 50, "User limit unchanged since -l not processed");
}

void test_mode_command_add_and_remove_op() {
	printCallerFunction(__FUNCTION__);
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 9);
	Client opTarget(&server, 10);
	opTarget.set_nick("opbuddy");

	const std::string ch_name = "#modetest3";
	server.addChannelMap(ch_name);
	Channel* channel = server.getChannel(ch_name);
	channel->addClient(&client);
	channel->addClient(&opTarget);
	channel->addOperator(&client);

	Command cmd;
	cmd.setName("MODE");
	cmd.addArg(ch_name);
	cmd.addArg("+o");
	cmd.addArg("opbuddy");

	modeCmd.execute(cmd, client);
	ASSERT_TRUE(channel->isOperator("opbuddy"), "opbuddy is promoted to operator");

	Command cmd2;
	cmd2.setName("MODE");
	cmd2.addArg(ch_name);
	cmd2.addArg("-o");
	cmd2.addArg("opbuddy");

	modeCmd.execute(cmd2, client);
	ASSERT_TRUE(!channel->isOperator("opbuddy"), "opbuddy is removed from operator list");
}

void test_mode_command_missing_parameter_error() {
	printCallerFunction(__FUNCTION__);
	Server server(0, 6667, "pass");
	ModeCommand modeCmd(&server);
	Client client(&server, 11);
	client.set_nick("Frank");
	const std::string ch_name = "#modetest4";
	server.addChannelMap(ch_name);
	Channel* channel = server.getChannel(ch_name);
	channel->addClient(&client);
	channel->addOperator(&client);

	Command cmd;
	cmd.setName("MODE");
	cmd.addArg(ch_name);
	cmd.addArg("+k");

	modeCmd.execute(cmd, client);

	ASSERT_TRUE(!client.isOutBufferEmpty(), "Missing parameter error message written");
	std::cout << "===outBuffer content:\n" << client.getNextOutBufferChunk();
}

int main() {
	std::cout << "Running ModeCommand tests..." << std::endl;
	test_mode_command_basic();
	test_mode_command_no_args();
	test_mode_command_invalid_target();
	test_mode_command_nonexistent_channel();
	test_mode_command_print_modes();
	std::cout << "\n\n=====\nhandleChannelMode function tests\n" << std::endl;
	test_mode_command_set_invite_only();
	test_mode_command_set_topic_restricted();

	test_mode_command_set_password_and_limit();
	test_mode_command_password_and_remove_limit();
	test_mode_command_add_and_remove_op();
	test_mode_command_missing_parameter_error();

	return 0;
}