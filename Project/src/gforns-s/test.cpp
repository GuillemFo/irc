/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 08:10:46 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/08 15:10:07 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BACKLOG 10
#define BUFFER_SIZE 1024

void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        std::perror("fcntl F_GETFL");
        std::exit(1);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::perror("fcntl F_SETFL");
        std::exit(1);
    }
}

int main() {
    int server_fd, epoll_fd;
    struct sockaddr_in server_addr;

    // 1. Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::perror("socket");
        return 1;
    }

    // 2. Set server socket to non-blocking
    setNonBlocking(server_fd);

    // 3. Bind server socket
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::perror("bind");
        return 1;
    }

    // 4. Listen
    if (listen(server_fd, BACKLOG) == -1) {
        std::perror("listen");
        return 1;
    }

    // 5. Create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::perror("epoll_create1");
        return 1;
    }

    // 6. Register server_fd to epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        std::perror("epoll_ctl: server_fd");
        return 1;
    }

    std::cout << "Server started on port " << PORT << std::endl;

    struct epoll_event events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];

    // 7. Event loop
    while (true) {
        int n_ready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n_ready == -1) {
            std::perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n_ready; ++i) {
            int fd = events[i].data.fd;

            if (fd == server_fd) {
                // 8. Accept new client
                int client_fd = accept(server_fd, NULL, NULL);
                if (client_fd == -1) {
                    std::perror("accept");
                    continue;
                }
                setNonBlocking(client_fd);

                // Register client socket
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd; /// Working on this at main.cpp line 154 08/04/25 17.10
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    std::perror("epoll_ctl: client_fd");
                    close(client_fd);
                    continue;
                }

                std::cout << "New client connected: fd " << client_fd << std::endl;
            } else {
                // 9. Handle client data
                ssize_t count = read(fd, buffer, BUFFER_SIZE);
                if (count == -1) {
                    std::perror("read");
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                } else if (count == 0) {
                    std::cout << "Client disconnected: fd " << fd << std::endl;
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                } else {
                    buffer[count] = '\0';
                    std::cout << "Received from " << fd << ": " << buffer;
                    // Echo back
                    ssize_t sent = send(fd, buffer, count, 0);
                    if (sent == -1) {
                        std::perror("send");
                        close(fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                    }
                }
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}

