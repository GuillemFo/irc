/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutBuffer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 22:00:55 by josegar2          #+#    #+#             */
/*   Updated: 2025/04/24 12:18:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OutBuffer.hpp"

OutBuffer::OutBuffer() : _offset(0) {}

OutBuffer::~OutBuffer() {}

void OutBuffer::addMessage(std::string &theMessage)
{
	this->_queue.push(theMessage);
}

std::string OutBuffer::getMessage()
{
	if (this->_queue.empty())
		return "";
	const std::string& current = this->_queue.front();
	return current.substr(_offset);

}

void OutBuffer::addOffset(size_t charsSent)
{
	this->_offset += charsSent;
	if (this->_offset >= this->_queue.front().size())
	{
		this->_queue.pop();
		this->_offset = 0;
	}
}

bool OutBuffer::isEmpty() const
{
	return this->_queue.empty();
}

void OutBuffer::clear()
{
	while (!this->_queue.empty())
	{
		this->_queue.pop();
	}
	this->_offset = 0;
}
