/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/08 12:24:28 by emaillet         ###   ########.fr       */
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
	this->_password = "";
	this->_needInvite = false;
	this->_needPassword = false;
	this->_needOpTopic = false;
	this->_maxClient = 0;
}

// Default destructor
Channel::~Channel(void) {
	this->_inviteList.empty();
	this->_joinedList.empty();
	this->_opList.empty();
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

void Channel::setNeedPassword(bool val) {
	this->_needPassword = val;
}


/* ************************************************************************** */
/* All members functions */
/* ************************************************************************** */


void Channel::Broadcast(Client& sender, std::string msg, broadcast type) {
	if (this->findClientJoin(sender) == -1) {
		throwRFCException(ERR_NOTONCHANNEL, this->getName(), sender.getNickname());
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
			std::string myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " JOIN " + this->getName() + "\r\n";
			send(static_cast<Client*>(*it)->getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
		}
		else if (type == BRCST_KICK || type == BRCST_DEOP || type == BRCST_OP || type == BRCST_TOPIC || type == BRCST_MODE)
			send(static_cast<Client*>(*it)->getUid(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
		it++;
	}
}

//Channel command to join
void Channel::Join(Client& client, Server& server, std::string& pass) {

	int clientPos = this->findClientJoin(client);
	if (clientPos != -1)
		return; // Déjà dans le channel

	if (this->_maxClient != 0 && (int)this->_joinedList.size() >= this->_maxClient)
		throwRFCException(ERR_CHANNELISFULL, this->getName(), client.getNickname());
	if (this->_needInvite && this->findClientInvite(client) == -1)
		throwRFCException(ERR_INVITEONLYCHAN, this->getName(), client.getNickname());
	if (this->_needPassword && this->_password != pass) {
		throwRFCException(ERR_BADCHANNELKEY, this->getName(), client.getNickname());
	}
	this->_joinedList.push_back(&client);

	this->Broadcast(client, "", BRCST_JOIN);

	if (this->_joinedList[0] == &client)
		this->_opList.push_back(&client);

	std::string myMsg;
	if (this->_topic.empty())
		myMsg = ":" + server.getHost() + " 331 " + client.getNickname() + " " + this->getName() + " :No topic is set\r\n";
	else
		myMsg = ":" + server.getHost() + " 332 " + client.getNickname() + " " + this->getName() + " :" + this->_topic + "\r\n";

	// Liste des clients
	myMsg += ":" + server.getHost() + " 353 " + client.getNickname() + " = " + this->getName() + " :";
	for (size_t i = 0; i < this->_joinedList.size(); ++i) {
		Client *c = this->_joinedList[i];
		if (this->findClientOp(*c) != -1)
			myMsg += "@";
		myMsg += c->getNickname();
		if (i + 1 < this->_joinedList.size())
			myMsg += " ";
	}
	myMsg += "\r\n:" + server.getHost() + " 366 " + client.getNickname() + " " + this->getName() + " :End of /NAMES list.\r\n";

	send(client.getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
}



//Channel command to kick
void Channel::Kick(std::string nick, std::string reason, bool leave, Client& sender) {
	int clientPos = this->findClientJoin(nick);
	if (clientPos >= 0) {

		std::string myMsg;
		if (!reason.empty()) {
		    myMsg = ":" + sender.getNickname() + " KICK " + this->getName() + " " + nick + " :" + reason + "\r\n";
		} else {
		    myMsg = ":" + sender.getNickname() + " KICK " + this->getName() + " " + nick + "\r\n";
		}
		if (leave == false)
			this->Broadcast(sender, myMsg, BRCST_KICK);
		this->_joinedList.erase(_joinedList.begin() + clientPos);
	}
	int opPos = this->findClientOp(nick);
	if (opPos != -1)
		this->_opList.erase(_opList.begin() + opPos);
}

//Channel command to add invite
void Channel::Invite(Client& client, Client& sender) {
	int clientPos = this->findClientInvite(client);
	if (clientPos == -1){
		std::string myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " INVITE " + client.getNickname() + " " + this->getName() + "\r\n";
		send(client.getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
		this->_inviteList.push_back(&client);
	}
}

//Channel command to remove invite
void Channel::DeInvite(Client& client, Server& server) {
	int clientPos = this->findClientInvite(client);
	if (clientPos >= 0)
		this->_inviteList.erase(_inviteList.begin() + clientPos);
	(void)server;
}

//Channel command to add operator
void Channel::Op(Client& client, Client& sender) {
	int clientPos = this->findClientOp(client);
	if (clientPos == -1) {
		this->_opList.push_back(&client);
		std::string myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " +o " + client.getNickname() + "\r\n";		
		this->Broadcast(client, myMsg, BRCST_OP);
	}
}

//Channel command to remove operator
void Channel::DeOp(Client& client, Client& sender) {
	int clientPos = this->findClientOp(client);
	if (clientPos != -1) {
		this->_opList.erase(_opList.begin() + clientPos);
		std::string myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " -o " + client.getNickname() + "\r\n";
		this->Broadcast(client, myMsg, BRCST_DEOP);
	}
}

//Channel command to set topic
void Channel::Topic(std::string topic, Server& server, Client& sender) {
	if (this->_needOpTopic == true && this->findClientOp(sender) == -1)
		throwRFCException(ERR_CHANOPRIVSNEEDED, this->getName(), sender.getNickname());
	if (topic.length() < 255)
	{
		std::string myMsg = ":" + server.getHost() + " 332 " + sender.getNickname() + " " + this->_name + " :" + topic.c_str() + "\r\n";
		this->_topic = topic;
		this->Broadcast(sender, myMsg, BRCST_TOPIC);
	}
	else
		throw std::runtime_error("Topic too long");
}

//Channel commands
void Channel::Mode(Client& sender, std::string option) {
	int senderPos = this->findClientOp(sender);
	std::istringstream iss(option);
	std::string opt, target, channel, myMsg;
		iss >> channel >> opt >> target;
	if (opt == "+l") {
		int maxValue = std::atoi(target.c_str());
		if (maxValue > 0) {
			this->_maxClient = maxValue;
			myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " +l " + target + "\r\n";
			this->Broadcast(sender, myMsg, BRCST_MODE);
		}
		else {
			throwRFCException(ERR_UNKNOWNMODE, opt, sender.getNickname());
		}
	}
	else if (opt == "-l") {
		this->_maxClient = 0;
		myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " -l\r\n";
		this->Broadcast(sender, myMsg, BRCST_MODE);
	}
	else if (opt == "+t") {
		this->_needOpTopic = true;
		myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " +t\r\n";
		this->Broadcast(sender, myMsg, BRCST_MODE);
	}
	else if (opt == "-t") {
		this->_needOpTopic = false;
		myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " -t\r\n";
		this->Broadcast(sender, myMsg, BRCST_MODE);
	}
	else if (opt == "+i") {
		this->_needInvite = true;
		myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " +i\r\n";
		this->Broadcast(sender, myMsg, BRCST_MODE);
	}
	else if (opt == "-i") {
		this->_needInvite = false;
		myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " -i\r\n";
		this->Broadcast(sender, myMsg, BRCST_MODE);
	}
	else if (opt == "+k") {
		this->_needPassword = true;
		if (target.empty())
			throwRFCException(ERR_NEEDMOREPARAMS, opt, sender.getNickname());
		if (target.length() > 255)
			throw std::runtime_error("Password too long");
		this->setPassword(target);
		myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " +k " + target + "\r\n";
		this->Broadcast(sender, myMsg, BRCST_MODE);
	}
	else if (opt == "-k") {
		this->_needPassword = false;
		this->setPassword("");
		myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " MODE " + this->getName() + " -k\r\n";
		this->Broadcast(sender, myMsg, BRCST_MODE);
	}
	else if (!target.empty() && (opt == "+o" || opt == "-o")) {
		int targetPos = this->findClientJoin(target);
		if (targetPos == -1)
			throwRFCException(ERR_NOSUCHNICK, target, sender.getNickname());
		Client *target = this->_joinedList[targetPos];
		if (senderPos == -1)
			throwRFCException(ERR_NOTONCHANNEL, this->getName(), sender.getNickname());
		if (opt == "+o")
			this->Op(*target, sender);
		else if (opt == "-o")
			this->DeOp(*target, sender);
	}

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

//Client id in join vector list By nick
int Channel::findClientJoin(std::string nick) {
	std::vector<Client*>::iterator	it = this->_joinedList.begin();
	std::vector<Client*>::iterator	end = this->_joinedList.end();
	for(int i = 0; it != end; i++) {
		if (static_cast<Client*>(*it)->getNickname() == nick) {
			return (i);
		}
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

//Client id in op vector list by nick
int Channel::findClientOp(std::string nick) {
	std::vector<Client*>::iterator	it = this->_opList.begin();
	std::vector<Client*>::iterator	end = this->_opList.end();
	for(int i = 0; it != end; i++) {
		if (static_cast<Client*>(*it)->getNickname() == nick) {
			return (i);
		}
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
