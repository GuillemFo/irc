/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:10:39 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/11 21:23:50 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>

std::string replace_tool(std::string str, std::string to_replace, std::string _new);
bool	valid_port(const std::string &str);
std::string name_tolower(const std::string& name);
std::string int_to_string(int value);
std::vector<std::string> split_arg(const std::string& arg);
