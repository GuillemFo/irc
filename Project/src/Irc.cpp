/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:40:34 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/12 11:50:41 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

Irc::Irc(){}

Irc::~Irc(){}

Irc::Irc(const Irc &other)
{
	*this = other;
}

Irc &Irc::operator=(const Irc &other)
{
	if (this != &other)
	{
		this->pass = other.pass;
		this->port = other.port;
	}
	return (*this);
}