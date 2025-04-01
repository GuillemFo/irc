/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:21:26 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/01 15:27:40 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void		Client::set_nick(const std::string &str){this->_nick = str;}
const 		std::string	Client::get_nick()const {return (this->_nick);}
void		Client::set_user(const std::string &str){this->_user = str;}
const 		std::string	Client::get_user()const {return (this->_user);}
void		Client::set_auth(bool i){this->_auth = i;}
bool		Server::get_auth()const {return (this->auth);}
void		Client::set_reg(bool i){this->_reg = i;}
bool		Client::get_reg() const{return this->_reg;}

