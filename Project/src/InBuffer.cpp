/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InBuffer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:35:10 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/23 04:12:06 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InBuffer.hpp"

InBuffer::InBuffer() : _buffer() {}
InBuffer::InBuffer(const InBuffer& src) : _buffer(src._buffer) {}
InBuffer& InBuffer::operator=(const InBuffer& src) {
	if (this != &src)
		this->_buffer = src._buffer;
	return *this;
}

InBuffer::~InBuffer() {}

void InBuffer::append(const std::string &data) {
	this->_buffer += data;
}

bool InBuffer::hasCompleteCommand() const {
	return this->_buffer.find("\r\n") != std::string::npos;
}

std::string InBuffer::extractCommand() {
	size_t pos = this->_buffer.find("\r\n");
	if (pos == std::string::npos)
		return "";
	
	std::string line = this->_buffer.substr(0, pos);
	this->_buffer.erase(0, pos + 2);

	if (line.length() > 512)
	{
		throw std::runtime_error("IRC command is longer that 512 characters. \
Aborting");
	}
	
	return line;
}

bool InBuffer::overflow() const {
	return this->_buffer.size() > this->_maxSize;
}

void InBuffer::clear() {
	this->_buffer.clear();
}

const std::string& InBuffer::getRaw() const {
	return this->_buffer;
}
