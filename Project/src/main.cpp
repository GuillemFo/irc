/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/12 09:12:23 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <arpa/inet.h>


//MAX_EVENTS → how many FDs epoll_wait will return at once (not max clients).
//BUFFER_SIZE → how many bytes you read from a socket at once.


#define MAX_EVENTS 128
#define BUFFER_SIZE 512

void	setNonBlocking(int sv_fd)
{
	int flag = fcntl(sv_fd, F_GETFL, 0);
	if (flag == -1)
	{
		std::cerr << "fcntl F_GETFL error" << std::endl;
		std::exit(-1);
	}
	if (fcntl(sv_fd, F_SETFL, flag | O_NONBLOCK) == -1)
	{
		std::cerr << "fcntl F_SETFL error" << std::endl;
		std::exit(-1);
	}
}


void cleanupClient(Server &s, int fd)
{
	if (s.getClient(fd) != NULL)
		s.getClient(fd)->partAllChannels();
	s.rmClientMap(fd);
	epoll_ctl(s.get_epollFD(), EPOLL_CTL_DEL, fd, NULL);
	close(fd);
}

void handleNewConnection(Server &s)
{
	while (true)
	{
		struct sockaddr_in client_addr;
		socklen_t addr_len = sizeof(client_addr);
		int cl_fd = accept(s.get_serverFD(), (struct sockaddr*)&client_addr, &addr_len);
		if (cl_fd < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			std::cerr << "accept error" << std::endl;
			return;
		}
		setNonBlocking(cl_fd);
		struct epoll_event ev;
		memset(&ev, 0, sizeof(ev));
		ev.events = EPOLLIN;
		ev.data.fd = cl_fd;
		if (epoll_ctl(s.get_epollFD(), EPOLL_CTL_ADD, cl_fd, &ev) < 0)
		{
			std::cerr << "epoll_ctl: client fd error" << std::endl;
			close(cl_fd);
			continue;
		}
		s.addClientMap(cl_fd);
		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
		s.getClient(cl_fd)->set_ip(std::string(client_ip));
	}
}


bool isValidIRCMessage(const std::string &input) {
	if (input.size() > 512)
		return false;
	return true;
}

void handleRead(Server &s, int fd)
{
	char buffer[BUFFER_SIZE];
	Client *client = s.getClient(fd);
	if (!client || client == NULL) {
		std::cerr << "Invalid client fd: " << fd << std::endl;
		return;
	}
	while (true)
	{
		buffer[0] = '\0';
		ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
		if (bytes == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			cleanupClient(s, fd);
			break;
		}
		else if (bytes == 0)
		{
			cleanupClient(s, fd);
			break;
		}
		else
		{
			std::string tmp(buffer, bytes);
			client->_in.append(tmp);
				if (!isValidIRCMessage(client->_in.getRaw()))
				{
					client->sendMessage(client->get_nick() + " :Input line was too long\r\n");
					client->_in.clear();
					return ;
				}
			
			while (client->_in.hasCompleteCommand())
			{
				Parser parser;
				Command cmd = parser.parse(client->_in.extractCommand());
				Client *clientfd = s.getClient(fd);
				s._dispatcher.dispatch(cmd, *clientfd);
			}

			struct epoll_event ev;
			memset(&ev, 0, sizeof(ev));
			ev.events = EPOLLIN | EPOLLOUT;
			ev.data.fd = fd;
			epoll_ctl(s.get_epollFD(), EPOLL_CTL_MOD, fd, &ev);
		}
	}
}

void handleSend(Server &s, int fd)
{
	if (s.getClient(fd) == NULL || s.getClient(fd) == 0 )
	{
		return ;
	}
	if (s.getClient(fd)->isOutEmpty())
		return ;
	std::string msg = s.getClient(fd)->getOutMessage();
	ssize_t sent_bytes = send(fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);  // nosignal to protect from sending to a close socket
	if (sent_bytes == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return; // try again later, wait for epollout
		std::cerr << "send error" << std::endl;
		cleanupClient(s, fd);
		return;
	}
	msg.erase(0, sent_bytes); // with the count of sent bytes we remove that number from the out buffer and continue
	s.getClient(fd)->popCharsSent(sent_bytes);
	if (s.getClient(fd)->isOutEmpty())
	{
		struct epoll_event ev;
		memset(&ev, 0, sizeof(ev));
		ev.events = EPOLLIN;
		ev.data.fd = fd;
		epoll_ctl(s.get_epollFD(), EPOLL_CTL_MOD, fd, &ev);
		return ;
	}
}

void handle_sigint(int sig)
{
	(void) sig;
	Server::mustExit = true;
}

void set_signals() {
	signal(SIGINT, handle_sigint);
	signal(SIGTERM, handle_sigint);
	signal(SIGQUIT, handle_sigint);
	signal(SIGPIPE, SIG_IGN);
}

void bind_server(Server &s) {
	s.set_server_name("127.0.0.1");
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // set IPv4 family
	server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
	server_addr.sin_port = htons(s.get_port()); // convert port to network byte order
	//Binding: 
	if (bind(s.get_serverFD(), (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
        close(s.get_serverFD());
		throw std::runtime_error("Bind error. Port already in use.");
	}
	//Listen:
	if(listen(s.get_serverFD(), HOLD_NON_ACCEPTED) < 0)
	{
        close(s.get_serverFD());
		throw std::runtime_error("listen error");
	}
}

void    set_epoll(Server &s) {
    int epoll_fd;
	//Creating epoll instance
	epoll_fd = epoll_create1(0);
	if (epoll_fd < 0)
	{
        close(s.get_serverFD());
		throw std::runtime_error("epoll_create error");
	}
	s.set_epollFD(epoll_fd);
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = s.get_serverFD();
		
	if (epoll_ctl(s.get_epollFD(), EPOLL_CTL_ADD, s.get_serverFD(), &ev) < 0)
	{
        close(s.get_serverFD());
		throw std::runtime_error("epoll_ctl: server_fd error");
	}
}

bool Server::mustExit = false;

int main(int ac, char **av)
{
	int sv_fd;
	try
	{
        if (ac != 3)
            throw std::runtime_error ("Wrong arguments");
		if (valid_port(av[1]) == false)
			throw std::runtime_error ("Invalid port");
 
        set_signals();

        sv_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (sv_fd < 0)
			throw std::runtime_error ("Socket error");
		
		setNonBlocking(sv_fd);
		Server s(sv_fd, atoi(av[1]), av[2]);
		
		if (!s.registerAllCommands())
			throw std::runtime_error ("Unable to register Command Handlers");

        bind_server(s);

        set_epoll(s);

		std::cout << "Server started on port " << C_R << s.get_port() << C_RESET << std::endl;
		std::cout << "Server started on pass " << C_R << av[2] << C_RESET << std::endl;

		struct epoll_event events[MAX_EVENTS];

		//Event loop
		while (!Server::mustExit)
		{
			int num_ready = epoll_wait(s.get_epollFD(), events, MAX_EVENTS, -1);
			if (num_ready < 0)
				break;

			for (int i = 0; i < num_ready; ++i)
			{
				int fd = events[i].data.fd;
				uint32_t ev = events[i].events;
				if (fd == s.get_serverFD())
					handleNewConnection(s);
				else
				{
					if (ev & EPOLLIN)
						handleRead(s, fd);
					if (ev & EPOLLOUT)
					{
						handleSend(s, fd);
					}
				}
			}
		}
		close(s.get_serverFD());
		close(s.get_epollFD());
		return (0);
	}
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
        return (-1);
	}
	return (0);
}	



