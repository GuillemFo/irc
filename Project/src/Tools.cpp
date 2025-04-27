/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:07:49 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/27 20:07:54 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	valid_port(const std::string &str) //change to a better name
{
	if (!str.empty())
	{
		for (std::string::size_type i = 0; i < str.length(); ++i)
		{
			if (!std::isdigit(str[i]))
				return (false);
		}
		if (str.length() <= 5) // min port 0 max port 65535 //maybe ther is a function to protect this properly for common used ports that are protected
		{
			int res = std::atoi(str.c_str());
			if (res <= 65535 && res >= 0)
				return (true);
			else
				return (false);
		}
		else
			return (false);
	}
	else
		return (false);
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

// convert to lowercase nicknames and channel names
std::string name_tolower(const std::string& name) {
	std::string result;
		
	for (size_t i = 0; i < name.size(); ++i) {
		unsigned char c = name[i]; // Handle potential negative chars
		
		// RFC 1459 special case folding
		switch (c) {
			case '[':  result += '{'; break;
			case ']':  result += '}'; break;
			case '\\': result += '|'; break;
			case '^':  result += '~'; break; // Common extension
			case '{':  // These prevent double-folding

			default:
				// Standard ASCII folding
				if (c >= 'A' && c <= 'Z') {
					result += c + ('a' - 'A');
				} else {
					result += c;
				}
		}
	}
	return result;
}

std::vector<std::string> split_arg(const std::string& arg) {
	std::vector<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type end = arg.find(',');

	while (end != std::string::npos) {
		result.push_back(arg.substr(start, end - start));
		start = end + 1;
		end = arg.find(',', start);
	}

	// Add the last element
	result.push_back(arg.substr(start));

	return result;
}