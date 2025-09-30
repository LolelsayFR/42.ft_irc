/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 19:45:02 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Channel/Channel.hpp"
#include "Errors/Exception.hpp"

/* ************************************************************************** */
/* All constructors and the destructor */
/* ************************************************************************** */

// Default constructor
Channel::Channel(void) : _password("") {
}

// Copy constructor
Channel::Channel(const Channel& other) : _name(other._name) {
	(void)other;
}

// Assignation constructor
Channel::Channel(std::string name) : _name(name) {}

// Default destructor
Channel::~Channel(void) {
}


/* ************************************************************************** */
/* All operator overload */
/* ************************************************************************** */

// Copy Operator
Channel& Channel::operator=(const Channel& other) {
	(void)other;
	return (*this);
}

/* ************************************************************************** */
/* All members functions */
/* ************************************************************************** */

//Channel commands
void Channel::Join(Client& client) {
	if (this->_joinedList.empty()) {
		this->_opList.push_back(&client);
	}	
	this->_joinedList.push_back(&client);
}

//Channel commands
void Channel::Leave(Client& client) {
	if (this->_joinedList.empty()) {
		this->_opList.push_back(&client);
	}
	this->_joinedList.erase(this->findClientJoin(client));
}

//Channel commands
void Channel::Kick(Client& client) {
	
}

//Channel commands
void Channel::Invite(Client& client) {
	
}

//Channel commands
void Channel::Op(Client& client) {
	
}

//Channel commands
void Channel::DeOp(Client& client) {
	
}

//Channel commands
void Channel::Topic(std::string topic) {
	
}

//Channel commands
void Channel::Mode(std::string option) {
	
}


std::vector<Client*>::iterator Channel::findClientJoin(Client& client) {
	std::vector<Client*>::iterator	it = this->_joinedList.begin();
	std::vector<Client*>::iterator	end = this->_joinedList.end();
	while(it != end) {
		if (*it == &client)
			return (it);
		it++;
	}
	return (end);
}

std::vector<Client*>::iterator Channel::findClientOp(Client& client) {
	std::vector<Client*>::iterator	it = this->_opList.begin();
	std::vector<Client*>::iterator	end = this->_opList.end();
	while(it != end) {
		if (*it == &client)
			return (it);
		it++;
	}
	return (end);
}

std::vector<Client*>::iterator Channel::findClientInvite(Client& client) {
	std::vector<Client*>::iterator	it = this->_inviteList.begin();
	std::vector<Client*>::iterator	end = this->_inviteList.end();
	while(it != end) {
		if (*it == &client)
			return (it);
		it++;
	}
	return (end);
}


/* ************************************************************************** */
/* End of file */
/* ************************************************************************** */