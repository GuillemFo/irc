/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:44:56 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/23 18:18:59 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"

Command::Command() {}
Command::Command(const Command& src)
	: _name(src._name), _args(src._args) {
}
Command& Command::operator=(const Command& src) {
	if (this != &src) {
		this->_name = src._name;
		this->_args = src._args;
	}
	return *this;
}
Command::~Command() {}
void Command::setName(const std::string& name) {
	this->_name = name;
}

void Command::addArg(const std::string& arg) {
	this->_args.push_back(arg);
}

const std::string& Command::getName() const {
	return this->_name;
}

const std::vector<std::string>& Command::getArgs() const {
	return this->_args;
}

void Command::printCommand() const {
	std::cout << "Command: " << this->getName() << std::endl;
	const std::vector<std::string>& args = this->getArgs();
	for (size_t i = 0; i < args.size(); i++) {
		std::cout << "Arg[" << i << "]: " << args[i] << std::endl;
	}
}
