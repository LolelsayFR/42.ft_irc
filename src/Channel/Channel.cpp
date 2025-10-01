/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/01 17:19:01 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Channel/Channel.hpp"
#include "Client/Client.hpp"
#include "Errors/Exception.hpp"

# define DEBUG_PRINT 	false
# define WHI 			"\e[1;38;5;0;107m "
# define RES			" \e[0m"

/* ************************************************************************** */
/* All constructors and the destructor */
/* ************************************************************************** */

// Assignation constructor
Channel::Channel(std::string name) : _name(name) {
	if (DEBUG_PRINT)
		std::cout << WHI"Channel" << RES << " name assignement constructor called : " << name << std::endl;	
	this->_topic = "";
	this->_password = "";
	this->_needInvite = false;
	this->_needPassword = false;
}

// Default destructor
Channel::~Channel(void) {
		if (DEBUG_PRINT)
		std::cout << WHI"Channel" << RES << " destructor called : " << _name << std::endl;	
}


/* ************************************************************************** */
/* All operator overload */
/* ************************************************************************** */

//Ostream insertion operator
std::ostream& operator<<(std::ostream& o, Channel& c) {
	Client* ptr;
	o << "/* Channel View ******************************************************** */" << std::endl;
	{
		std::vector<Client*> list = c.getJoinList();
		std::vector<Client*>::iterator	it = list.begin();
		std::vector<Client*>::iterator	end = list.end();
		o << "Channel name : " << c.getName() << std::endl;
		if (c.getNeedPassword())
			o << "Need password : true, the pass is '" << c.getPassword() << "'" << std::endl; 
		else
			o << "Need password : false" << std::endl; 
		o << "Client topic : " << std::endl;
		o << "/* Client list from channel ******************************************** */" << std::endl;
		for (int i = 0; it != end; i++, it++) {
			ptr = *it;
			{//BLOC A METRE DANS L'OVERLOAD CLIENT
			o 	<< "| n°" << std::setw(5) << i
				<< " | Username = " <<  std::setw(10) << ptr->getUsername()
				<< " | Nickname = " <<  std::setw(10) << ptr->getNickname()
				<< " | Is op = " << ((c.findClientOp(*ptr) == -1) ? ("false") : ("true "))
				<< " |"  << std::endl;	
			}	
		}
		o << "/* ********************************************************************* */" << std::endl;
	}
	if (!c.getNeedInvite())
		return (o);
	{
		std::vector<Client*> list = c.getInviteList();
		std::vector<Client*>::iterator	it = list.begin();
		std::vector<Client*>::iterator	end = list.end();
		o << "/* Client invitation list from channel ********************************* */" << std::endl;
		for (int i = 0; it != end; i++, it++) {
			ptr = *it;
			o 	<< "| n°" << std::setw(5) << i
				<< " | Username = " <<  std::setw(10) << ptr->getUsername()
				<< " | Nickname = " <<  std::setw(10) << ptr->getNickname()
				<< " |"  << std::endl;		
		}
		o << "/* ********************************************************************* */" << std::endl;
	}
	return (o);
}

/* ************************************************************************** */
/* All getter */
/* ************************************************************************** */

//Channel JoinList getter
const std::vector<Client*>&	Channel::getJoinList(void) const {
	return (this->_joinedList);
}

//Channel OpList getter
const std::vector<Client*>&	Channel::getOpList(void) const {
	return (this->_opList);
}

//Channel InviteList getter
const std::vector<Client*>&	Channel::getInviteList(void) const {
	return (this->_inviteList);
}

//Channel name getter
const std::string Channel::getName(void) const {
	return (this->_name);
}

//Channel Topic getter
const std::string Channel::getTopic(void) const {
	return (this->_topic);
}


//Channel JoinList getter
const std::string Channel::getPassword(void) const {
	return (this->_password);
}

//Channel NeedInvite bool getter
bool Channel::getNeedInvite(void) const {
	return (this->_needInvite);
}

//Channel NeedPassword bool getter
bool Channel::getNeedPassword(void) const {
	return (this->_needPassword);
}

//Password setter
void Channel::setPassword(std::string pass) {
	this->_password = pass;
}


/* ************************************************************************** */
/* All members functions */
/* ************************************************************************** */


//Channel command to join
void Channel::Join(Client& client) {
	int clientPos = this->findClientJoin(client);
	if (DEBUG_PRINT)
		std::cout << WHI"CHANNEL INTERN Join" << RES << " Try : " << client.getUsername() << " | Pos : " << clientPos << std::endl;	
	if (this->_joinedList.empty()) {
		this->_opList.push_back(&client);
	}	
	if (clientPos == -1)
		this->_joinedList.push_back(&client);
}

//Channel command to kick
void Channel::Kick(Client& client) {
	int clientPos = this->findClientJoin(client);
	if (DEBUG_PRINT)
		std::cout << WHI"CHANNEL INTERN Kick" << RES << " Try : " << client.getUsername() << " | Pos : " << clientPos << std::endl;	
	if (clientPos >= 0)
		this->_joinedList.erase(_joinedList.begin() + clientPos);
}

//Channel command to add invite
void Channel::Invite(Client& client) {
	int clientPos = this->findClientInvite(client);
	if (DEBUG_PRINT)
		std::cout << WHI"CHANNEL INTERN Invite" << RES << " Try : " << client.getUsername() << " | Pos : " << clientPos << std::endl;	
	if (clientPos == -1)
		this->_inviteList.push_back(&client);
}

//Channel command to remove invite
void Channel::DeInvite(Client& client) {
	int clientPos = this->findClientInvite(client);
	if (DEBUG_PRINT)
		std::cout << WHI"CHANNEL INTERN Deinvite" << RES << " Try : " << client.getUsername() << " | Pos : " << clientPos << std::endl;	
	if (clientPos >= 0)
		this->_inviteList.erase(_inviteList.begin() + clientPos);
}

//Channel command to add operator
void Channel::Op(Client& client) {
	int clientPos = this->findClientOp(client);
	if (DEBUG_PRINT)
		std::cout << WHI"CHANNEL INTERN Op" << RES << " Try : " << client.getUsername() << " | Pos : " << clientPos << std::endl;	
	if (clientPos == -1)
		this->_opList.push_back(&client);
}

//Channel command to remove operator
void Channel::DeOp(Client& client) {
	int clientPos = this->findClientOp(client);
	if (DEBUG_PRINT)
		std::cout << WHI"CHANNEL INTERN Deop" << RES << " Try : " << client.getUsername() << " | Pos : " << clientPos << std::endl;		
	if (clientPos >= 0)
		this->_opList.erase(_opList.begin() + clientPos);
}

//Channel command to set topic
void Channel::Topic(std::string topic) {
	if (topic.length() < 256)
		this->_topic = topic;
}

//Channel commands
void Channel::Mode(std::string option) {
	(void)option;
}

/* ************************************************************************** */
/* Utils member function */
/* ************************************************************************** */

//Client id in join vector list
int Channel::findClientJoin(Client& client) {
	std::vector<Client*>::iterator	it = this->_joinedList.begin();
	std::vector<Client*>::iterator	end = this->_joinedList.end();
	for(int i = 0; it != end; i++) {
		if (*it == &client)
			return (i);
		it++;
	}
	return (-1);
}

//Client id in op vector list
int Channel::findClientOp(Client& client) {
	std::vector<Client*>::iterator	it = this->_opList.begin();
	std::vector<Client*>::iterator	end = this->_opList.end();
	for(int i = 0; it != end; i++) {
		if (*it == &client)
			return (i);
		it++;
	}
	return (-1);
}

//Client id in invite vector list
int Channel::findClientInvite(Client& client) {
	std::vector<Client*>::iterator	it = this->_inviteList.begin();
	std::vector<Client*>::iterator	end = this->_inviteList.end();
	for(int i = 0; it != end; i++) {
		if (*it == &client)
			return (i);
		it++;
	}
	return (-1);
}


/* ************************************************************************** */
/* End of file */
/* ************************************************************************** */