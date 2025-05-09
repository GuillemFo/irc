/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:10:39 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/07 00:07:03 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>

std::string to_lower(std::string &str);
std::string replace_tool(std::string str, std::string to_replace, std::string _new);
std::string to_upper(std::string &str);
bool	valid_port(const std::string &str);
std::string name_tolower(const std::string& name);
std::string int_to_string(int value);
std::vector<std::string> split_arg(const std::string& arg);
