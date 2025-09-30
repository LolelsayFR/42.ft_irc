/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:34:55 by arthur            #+#    #+#             */
/*   Updated: 2025/09/30 16:08:34 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Client/Client.hpp"

Client::Client(int fd) : _uid(fd){
	std::cout << "New client connected with fd: " << fd << std::endl;
}

Client::~Client(void) {
	std::cout << "Client with fd: " << this->_uid << " disconnected." << std::endl;
}

std::string Client::getUsername(void)const{
	return (this->_username);
}

void Client::setUsername(std::string username){
	this->_username = username;
}
