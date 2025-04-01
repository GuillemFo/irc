/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:34:54 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/01 12:18:21 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Client.hpp"

class Client ;

class Channel
{
	protected:
		std::string					_name;
		std::string					_topic;
		std::string					_pass;	// if empty no pass need
		std::map<int , Client *>	_client_map; //int should be the fd for that client established by epoll() Idea 01/04/25 12.11
		
	public:
		Channel(std::string name);	//forcing to create a channel with name at least.
		Channel(std::string name, std::string password);
		~Channel();
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

	


};
