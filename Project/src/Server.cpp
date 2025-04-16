/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:40:34 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/16 09:37:46 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int sv_fd, int port, std::string sv_pass) : _sv_fd(sv_fd), _port(port), _sv_pass(sv_pass){}

Server::~Server()
{
	// loop to delete all clients and channels due new[] in each map!!
	// delete ->second (as the exam :) )
	// dont forget to .clear _cl_map and _ch_map


}

Server::Server(const Server &other){*this = other;}	//no need??

Server &Server::operator=(const Server &other)	//no need??
{
	if (this != &other)
	{
		this->_sv_pass = other._sv_pass;
		this->_port = other._port;
	}
	return (*this);
}

///////////////////////////// OUR FUNCTIONS ////////////////////////////////////



int	Server::get_serverFD() {return (this->_sv_fd);}

void	Server::set_epollFD(int nb) {this->_epoll_fd = nb;}
int	Server::get_epollFD() {return (this->_epoll_fd);}

void	Server::set_server_name(const std::string &s){this->_sv_name = s;}


int	Server::get_port()const {return (this->_port);}

bool	Server::check_pass(const std::string &str)
{
	if (str == this->_sv_pass)
	{
		return (true);
	}
	return (false);
}


int	Server::addClientMap(int fd)
{
	if (_cl_map.find(fd) == _cl_map.end())
	{
		this->_cl_map.insert(std::pair<int, Client*>(fd, new Client(fd)));
		return (1);
	}
	else
		std::cout << "Client with fd " << fd << " already exists!" << std::endl;
	return (-1);
}



int	Server::addChannelMap(std::string &str)
{
	if (_ch_map.find(str) == _ch_map.end())
	{
		this->_ch_map.insert(std::pair<std::string, Channel*>(str, new Channel(str)));
		return (1);
	}
	else
		std::cout << "Channel with name " << str << " already exists!" << std::endl;
	return (-1);
}


int	Server::rmClientMap(int fd)
{
	if (_cl_map.find(fd) == _cl_map.end())
		std::cout << "Client with fd " << fd << " does not exists!" << std::endl;
	else
	{
		delete this->_cl_map[fd];
		this->_cl_map.erase(fd);
		return (1);
	}
	return (-1);
}



int	Server::rmChannelMap(std::string &str) // not needed by subject
{
	if (_ch_map.find(str) == _ch_map.end())
		std::cout << "Channel with name " << str << " does not exists!" << std::endl;
	else
	{
		
		delete this->_ch_map[str];
		this->_ch_map.erase(str);
		return (1);
	}
	return (-1);
}

const std::map<int, Client*>	&Server::getClientMap() const {return _cl_map;}

const std::map<std::string, Channel*>	&Server::getChannelMap() const {return _ch_map;}



// void	Server::welcome_msg(const std::string &nickname)
// {
// 	std::stringstream message;
// 	message << ":" << _sv_name << " 001 " << nickname << " :Welcome to our IRC server " << nickname << "!" << std::endl;
// 	this->send_out(message.str());
// 	message.clear();
// }

// int	Server::send_out(std::string message)	//redo 08/04/25 16.29
// {
// 	ssize_t bytes_sent = send(this->client_fd , message.c_str(), strlen(message.c_str()), 0);
// 	if (bytes_sent < 0)
// 	{
// 		std::cout << "Error sending message from server to client" << std::endl;
// 		close(this->_sv_fd);
// 		return (1);
// 	}
// 	return (0);
// }


//This will need to be redesigned

void Server::buff_to_string(char *str)
{
	//Prepare strings to be split when \r\n is found ??? 
	///////// Printing tool to show non printable //////////////////////////
	std::string test(str);
	test = replace_tool(test, "\r", "'/r'");
	test = replace_tool(test, "\n", "'/n'\n");
	std::cout << "buff_in:" << C_R << test << C_RESET << ":end"<< std::endl;
	////////////////////////////////////////////////////////////////////////

	std::string content;
	std::string line(str);
	size_t pos = line.find('\r');
	if (line[pos] == '\r' && line[pos +1] == '\n') //change 01/04/25 12.32 line[pos +1] == '\n' to check poss contains \r first
	{
		while (pos != std::string::npos)
		{
			if (line[pos] == '\r' && line[pos +1] == '\n')	//change 01/04/25 12.32 line[pos +1] == '\n' to check poss contains \r first
			{
				content = line.substr(0, pos);
				//this->command_list(content);
				line.erase(0, pos+2);
				pos = line.find('\r');
				if (!line.empty() && line[pos] == '\r' && line[pos +1] == '\n') //change 01/04/25 12.32 added check for line[pos] == '\r'
				{
					pos = line.find('\r');
				}
			}			
		}
		pos = line.find('\n');
		content = line.substr(0, pos);
		//this->command_list(content);
	}
	else
		throw std::string("Exiting");
}

