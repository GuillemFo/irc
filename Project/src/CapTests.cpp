/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapTests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:35:10 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/23 18:30:48 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include "CapCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) std::cerr << "FAIL: " << msg << std::endl; \
    else std::cout << "PASS: " << msg << std::endl; \
} while (0)

void test_cap_command_basic() {
    Server server(0, 6667, "pass");
    CapCommand capCmd(&server);
    ASSERT_TRUE(true, "CapCommand constructed with server pointer");
}

void test_cap_command_execute_ls() {
    Server server(0, 6667, "pass");
    CapCommand capCmd(&server);
    Client client(&server, 1);

    Command cmd;
    cmd.setName("CAP");
    cmd.addArg("LS");

    capCmd.execute(cmd, client);

    ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after CAP LS");
}

void test_cap_command_execute_end() {
    Server server(0, 6667, "pass");
    CapCommand capCmd(&server);
    Client client(&server, 2);

    Command cmd;
    cmd.setName("CAP");
    cmd.addArg("END");

    capCmd.execute(cmd, client);
    ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after CAP END");
}

void test_cap_command_execute_unknown() {
    Server server(0, 6667, "pass");
    CapCommand capCmd(&server);
    Client client(&server, 3);

    Command cmd;
    cmd.setName("CAP");
    cmd.addArg("UNKNOWN");

    capCmd.execute(cmd, client);
    ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty after CAP UNKNOWN");
}

void test_cap_command_missing_arg() {
    Server server(0, 6667, "pass");
    CapCommand capCmd(&server);
    Client client(&server, 4);

    Command cmd;
    cmd.setName("CAP");

    capCmd.execute(cmd, client);
    ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty for CAP with missing argument");
}

void test_cap_command_junk_input() {
    Server server(0, 6667, "pass");
    CapCommand capCmd(&server);
    Client client(&server, 5);

    Command cmd;
    cmd.setName("CAP");
    cmd.addArg("1234567890!@#$%^&*()");

    capCmd.execute(cmd, client);
    ASSERT_TRUE(!client.isOutBufferEmpty(), "Client buffer not empty for CAP with junk input");
}

int main() {
    std::cout << "Running CapCommand tests..." << std::endl;
    test_cap_command_basic();
    test_cap_command_execute_ls();
    test_cap_command_execute_end();
    test_cap_command_execute_unknown();
    test_cap_command_missing_arg();
    test_cap_command_junk_input();
    return 0;
}
