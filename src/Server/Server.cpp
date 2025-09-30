/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 13:54:09 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server/Server.hpp"

/* ************************************************************************** */
/* All constructors and the destructor */
/* ************************************************************************** */

// Default constructor
Server::Server(void) : _port(-1), _password("") {
}

// Copy constructor
Server::Server(const Server& other) : _port(-1), _password("") {
	(void)other;
}

// Assignation constructor
Server::Server(int port, std::string password) : _port(port), _password(password) {}

// Default destructor
Server::~Server(void) {
}


/* ************************************************************************** */
/* All operator overload */
/* ************************************************************************** */

// Copy Operator
Server& Server::operator=(const Server& other) {
	(void)other;
	return (*this);
}

/* ************************************************************************** */
/* All members functions */
/* ************************************************************************** */

void Server::start(void){
	std::cout << "Port : " << this->_port << " Password : " << this->_password << std::endl;
}
