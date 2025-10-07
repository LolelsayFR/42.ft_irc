/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/06 19:05:33 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Channel/Channel.hpp"
#include "Client/Client.hpp"
#include "Server/Server.hpp"
#include "Errors/Exception.hpp"

/* ************************************************************************** */
/* All constructors and the destructor */
/* ************************************************************************** */

// Assignation constructor
Channel::Channel(std::string name) : _name(name) {
	this->_topic = "";
	if (name == "#yo")
		_topic = "sakhjdfkjASHDKJHSAKJd";
	this->_password = "";
	this->_needInvite = false;
	this->_needPassword = false;
}

// Default destructor
Channel::~Channel(void) {
	this->_inviteList.empty();
	this->_joinedList.empty();
	this->_opList.empty();
}


/* ************************************************************************** */
/* All operator overload */
/* ************************************************************************** */

//Ostream insertion operator
std::ostream& operator<<(std::ostream& o, Channel& c) {
	Client* ptr;
	o << std::endl << WHI << "/* Channel View */" << RES << std::endl;
	{
		std::vector<Client*> list = c.getJoinList();
		std::vector<Client*>::iterator	it = list.begin();
		std::vector<Client*>::iterator	end = list.end();
		o << " /\tChannel name : " << c.getName() << std::endl;
		if (c.getNeedPassword())
			o << " /\tNeed password : true, the pass is '" << c.getPassword() << "'" << std::endl;
		else
			o << " /\tNeed password : false" << std::endl;
		o << " /\tChannel topic : " << c.getTopic() << std::endl;
		o << WHI << "/* Client in channel */" << RES << std::endl;
		for (int i = 0; it != end; i++, it++) {
			ptr = *it;
			{
			o 	<< " | n°" << std::setw(5) << i
				<< " | Username = " <<  std::setw(10) << ptr->getUsername()
				<< " | Nickname = " <<  std::setw(10) << ptr->getNickname()
				<< " | Is op = " << ((c.findClientOp(*ptr) == -1) ? ("false") : ("true "))
				<< " |"  << std::endl;
			}
		}
		if (list.empty())
			o << "/\tEmpty.." << std::endl;
		o << "/* ** */" << std::endl;
	}
	if (!c.getNeedInvite())
		return (o);
	{
		std::vector<Client*> list = c.getInviteList();
		std::vector<Client*>::iterator	it = list.begin();
		std::vector<Client*>::iterator	end = list.end();
		o << WHI << "/* Client invitation list from channel */" << RES << std::endl;
		if (list.empty())
			o << "\tEmpty.." << std::endl;
		else
			for (int i = 0; it != end; i++, it++) {
				ptr = *it;
				o 	<< "| n°" << std::setw(5) << i
					<< " | Username = " <<  std::setw(10) << ptr->getUsername()
					<< " | Nickname = " <<  std::setw(10) << ptr->getNickname()
					<< " |"  << std::endl;
			}
		o << "/* ** */" << std::endl;
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


void Channel::Broadcast(Client& sender, std::string msg, broadcast type) {
	if (this->findClientJoin(sender) == -1) {
		return ;
		//throwRFCException(ERR_NOTONCHANNEL, this->getName());
	}
	std::vector<Client*>::iterator	it = this->_joinedList.begin();
	std::vector<Client*>::iterator	end = this->_joinedList.end();
	for(int i = 0; it != end; i++) {
		if (*it != &sender && type == BRCST_PRVMSG)
			static_cast<Client*>(*it)->receptMessage(*this, sender, msg);
		else if (type == BRCST_LEAVE)
			static_cast<Client*>(*it)->leaveChannel(sender, *this);
		else if (type == BRCST_LEAVE_MSG)
			static_cast<Client*>(*it)->leaveChannel(sender, msg, *this);
		else if (type == BRCST_JOIN) {
			std::string myMsg = ":" + sender.getNickname() + " JOIN " + this->getName() + "\r\n";
			send(static_cast<Client*>(*it)->getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
		}
		else if (type == BRCST_OP) {
			std::string myMsg = ":" + SERVERNAME + " MODE " + this->getName() + " +o " + sender.getNickname() + "\r\n";
			send(static_cast<Client*>(*it)->getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
		}
		it++;
	}
}

//Channel command to join
void Channel::Join(Client& client) {
	int clientPos = this->findClientJoin(client);
	if (clientPos == -1) {
	    std::vector<Client*>::iterator it = this->_inviteList.begin();
	    std::vector<Client*>::iterator end = this->_inviteList.end();
		this->_joinedList.push_back(&client);
		if (this->_joinedList[0] == &client) {
			this->Op(client);
		}
		this->Broadcast(client, "", BRCST_JOIN);
		std::string myMsg = ":" + SERVERNAME + " 331 " + client.getNickname() + " " + this->getName() + " :No topic is set\r\n";
		if (!this->_topic.empty())
    		myMsg = ":" + SERVERNAME + " 332 " + client.getNickname() + " " + this->getName() + " :" + this->_topic + "\r\n";
		myMsg += ":" + SERVERNAME + " 353 " + client.getNickname() + " = " + this->getName() + " :";
		myMsg += client.getNickname() + " ";
	    for (; it != end; ++it)
	        myMsg += static_cast<Client*>(*it)->getNickname() + " ";
	    if (!this->_inviteList.empty())
	        myMsg.erase(myMsg.size() - 1);
	    myMsg += "\r\n";
		myMsg += ":" + SERVERNAME + " 366 " + client.getNickname() + " " + this->getName() + " :End of /NAMES list.\r\n";	
		send(client.getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
	}
}

//Channel command to kick
void Channel::Kick(Client& client) {
	int clientPos = this->findClientJoin(client);
	if (clientPos >= 0)
		this->_joinedList.erase(_joinedList.begin() + clientPos);
}

//Channel command to add invite
void Channel::Invite(Client& client) {
	int clientPos = this->findClientInvite(client);
	if (clientPos == -1)
		this->_inviteList.push_back(&client);
}

//Channel command to remove invite
void Channel::DeInvite(Client& client) {
	int clientPos = this->findClientInvite(client);
	if (clientPos >= 0)
		this->_inviteList.erase(_inviteList.begin() + clientPos);
}

//Channel command to add operator
void Channel::Op(Client& client) {
	int clientPos = this->findClientOp(client);
	if (clientPos == -1) {
		this->_opList.push_back(&client);
		this->Broadcast(client, "", BRCST_OP);
	}
}

//Channel command to remove operator
void Channel::DeOp(Client& client) {
	int clientPos = this->findClientOp(client);
	if (clientPos >= 0)
		this->_opList.erase(_opList.begin() + clientPos);
}

//Channel command to set topic
void Channel::Topic(std::string topic) {
	if (topic.length() < 256)
		this->_topic = topic;
}

//Channel commands
void Channel::Mode(Client& sender , std::string option) {
	int clientPos = this->findClientOp(sender);
	if (clientPos == -1)
		;//throwRFCException(ERR_NOTONCHANNEL, this->getName());
	//if (option)
	std::cout << option  + "----------------------------------------------------------" << std::endl;
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