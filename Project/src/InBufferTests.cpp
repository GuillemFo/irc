/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InBufferTests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:35:10 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/23 04:11:43 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cassert>
#include "../Include/InBuffer.hpp"

void test_basic_append_and_extract() {
    InBuffer buf;
    buf.append("JOIN #general\r\n");

    assert(buf.hasCompleteCommand());
    std::string cmd = buf.extractCommand();
    assert(cmd == "JOIN #general");
    assert(!buf.hasCompleteCommand());
    std::cout << "[✓] Basic append and extract test passed.\n";
}

void test_partial_message() {
    InBuffer buf;
    buf.append("PRIVMSG #chan :Hel");

    assert(!buf.hasCompleteCommand());
    buf.append("lo!\r\n");
    assert(buf.hasCompleteCommand());

    std::string cmd = buf.extractCommand();
    assert(cmd == "PRIVMSG #chan :Hello!");
    std::cout << "[✓] Partial message test passed.\n";
}

void test_multiple_messages() {
    InBuffer buf;
    buf.append("PING\r\nPONG\r\n");

    assert(buf.hasCompleteCommand());
    assert(buf.extractCommand() == "PING");

    assert(buf.hasCompleteCommand());
    assert(buf.extractCommand() == "PONG");

    assert(!buf.hasCompleteCommand());
    std::cout << "[✓] Multiple messages test passed.\n";
}

void test_oversized_message() {
    InBuffer buf;
    std::string longLine(513, 'A'); // 513 'A's, too long

    buf.append(longLine + "\r\n");

    try {
        buf.extractCommand();
        assert(false); // Should never reach here
    } catch (const std::runtime_error& e) {
        std::cout << "[✓] Oversized message test passed: " << e.what() << '\n';
    }
}

void test_buffer_overflow() {
    InBuffer buf;
    std::string spam(9000, 'X'); // way over max buffer size

    buf.append(spam);
    assert(buf.overflow());
    std::cout << "[✓] Buffer overflow test passed.\n";
}

int main() {
    test_basic_append_and_extract();
    test_partial_message();
    test_multiple_messages();
    test_oversized_message();
    test_buffer_overflow();

    std::cout << "\nAll InBuffer tests passed.\n";
    return 0;
}