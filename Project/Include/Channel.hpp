/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:34:54 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/01 09:05:07 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"

class Client ;

class Channel
{
	protected:
		std::string _name;
		std::string _topic;
		std::string _pass;	// if empty no pass need
		bool		pw_bool;
		std::map<int , Client *> _clients; 
	public:
	


};
