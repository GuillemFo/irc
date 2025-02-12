/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:09 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/12 11:53:18 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <exception>
#include <iostream>
#include <sstream>

class Irc
{
	protected:
		int port;
		std::string pass;
	public:
		Irc();
		Irc(std::string str, int nb);
		~Irc();
		Irc(const Irc &other);
		Irc		&operator=(const Irc &other);
		void	set_port(int nb);
		void	set_pass(std::string str);
		void	store_port(std::string str);
		void	store_pass(std::string str);
		
};


