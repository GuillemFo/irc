/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:27:19 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/10 10:05:52 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) : _chName(name) {}

Channel::Channel(std::string name, std::string pass) : _chName(name), _chPass(pass) {}

Channel::~Channel() {}

Channel::Channel(const Channel &other) {*this = other;}

Channel 			&Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		this->_chName = other._chName;
		this->_chPass = other._chPass;
		this->_chTopic = other._chTopic;
	}
	return (*this);
}

const std::string	Channel::get_name() const {return (this->_chName);}

void				Channel::set_topic(const std::string &str) {this->_chTopic = str;}

const std::string	Channel::get_topic() const {return (this->_chTopic);}

void				Channel::set_pass(std::string &str) {this->_chPass = str;}

bool 				Channel::check_pass(std::string &str)
{
	if (str == this->_chPass)
	{
		return (true);
	}
	return (false);
}

void				Channel::set_userLimit(int nb) {this->_userLimit = nb;}

int					Channel::get_userLimit() {return (this->_userLimit);}

void				Channel::add_client(int fd) {this->_clients.insert(fd);}

std::set<int>		Channel::get_clients() const {return (_clients);}

void				Channel::add_op(int fd) {this->_operators.insert(fd);}

std::set<int>		Channel::get_ops() const {return (_operators);}

void				Channel::set_invOnly(bool nb) {this->_inviteOnly = nb;}

bool				Channel::get_invOnly() const {return (this->_inviteOnly);}