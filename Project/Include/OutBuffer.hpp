/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutBuffer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 21:46:43 by josegar2          #+#    #+#             */
/*   Updated: 2025/04/17 22:37:48 by josegar2         ###   ########.fr       */
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