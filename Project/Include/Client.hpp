/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:35:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/01 09:06:01 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

// should clients be init with new[]??
class Client
{
	private:
		std::string _nick;
		std::string _name;
		int _client_fd;

	public:

};
