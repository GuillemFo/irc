/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:07:49 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/07 11:10:11 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	check_port(const std::string &str) //change to a better name
{
	if (!str.empty())
	{
		for (std::string::size_type i = 0; i < str.length(); ++i)
		{
			if (!std::isdigit(str[i]))
				throw std::string("Non digit for port");
		}
		if (str.length() <= 5) // min port 0 max port 65535 //maybe ther is a function to protect this properly for common used ports that are protected
		{
			int res = std::atoi(str.c_str());
			if (res <= 65535 && res >= 0)
				return(res);
			else
				throw std::string("Invalid port");
		}
		else
				throw std::string("Invalid port");
	}
	else
		throw std::string("Empty string");
}

std::string to_upper(std::string &str)
{
	std::stringstream ss;
	for (int i = 0; str[i] != '\0'; i++)
	{
		ss << (char)std::toupper(str[i]);
	}
		std::string ret = ss.str();
	return (ret);
}
std::string to_lower(std::string &str)
{
	std::stringstream ss;
	for (int i = 0; str[i] != '\0'; i++)
	{
		ss << (char)std::tolower(str[i]);
	}
		std::string ret = ss.str();
	return (ret);
}

std::string replace_tool(std::string str, std::string to_replace, std::string _new)
{
	size_t pos = 0;
	while ((pos = str.find(to_replace, pos)) != std::string::npos)
	{
		str.replace(pos, to_replace.length(), _new);
		pos = (pos  + _new.length());
	}
	return (str);
}


