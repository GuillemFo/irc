/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutBuffer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 21:46:43 by josegar2          #+#    #+#             */
/*   Updated: 2025/04/24 12:17:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <queue>
#include <string>

class OutBuffer
{
	private:
		std::queue<std::string> _queue;
		size_t _offset;
	public:
		OutBuffer();
		~OutBuffer();
		void addMessage(std::string &theMessage);
		std::string getMessage();
		void addOffset(size_t charsSent);
		bool isEmpty() const;
		void clear();
};
