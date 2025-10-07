/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/07 16:03:14 by emaillet         ###   ########.fr       */
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
	this->_maxClient = 0;
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


void Channel::Broadcast(Client& sender, std::string msg, broadcast type, Server& server) {
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
			std::string myMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getHostname() + " JOIN " + this->getName() + "\r\n";
			send(static_cast<Client*>(*it)->getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
		}
		else if (type == BRCST_KICK || type == BRCST_DEOP || type == BRCST_OP)
			send(static_cast<Client*>(*it)->getUid(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
		else if (type == BRCST_TOPIC)
			send(static_cast<Client*>(*it)->getUid(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
		it++;
	}
	(void)server;
}

//Channel command to join
void Channel::Join(Client& client, Server& server) {
	int clientPos = this->findClientJoin(client);
	if (clientPos == -1) {
		if (this->_maxClient != 0 && (int)this->_joinedList.size() >= this->_maxClient) {
			//throwRFCException(ERR_CHANNELISFULL, this->getName());
			return ;
		}
		if (this->_needInvite == true && this->findClientInvite(client) == -1) {
			//throwRFCException(ERR_INVITEONLYCHAN, this->getName());
			return ;
		}
		if (this->_needPassword == true) {
			//Password check pass
			//if (this->_password != pass)
				//throwRFCException(ERR_BADCHANNELKEY, this->getName());
		}
		this->_joinedList.push_back(&client);

		//Join Broadcast
		this->Broadcast(client, "", BRCST_JOIN, server);

		//Auto op
		if (this->_joinedList[0] == &client)
			this->_opList.push_back(&client);
		//Topic
		std::string myMsg = ":" + server.getHost() + " 331 " + client.getNickname() + " " + this->getName() + " :No topic is set\r\n";
		if (!this->_topic.empty())
    		myMsg = ":" + server.getHost() + " 332 " + client.getNickname() + " " + this->getName() + " :" + this->_topic + "\r\n";

		//List of client
		myMsg += ":" + server.getHost() + " 353 " + client.getNickname() + " = " + this->getName() + " :";
	    std::vector<Client*>::iterator it = this->_joinedList.begin();
	    std::vector<Client*>::iterator end = this->_joinedList.end();
	    for (; it != end; ++it) {
			if (this->findClientOp(*static_cast<Client*>(*it)) != -1)
				myMsg += "@";
	        myMsg += static_cast<Client*>(*it)->getNickname();
			if (it + 1 != end)
				myMsg += " ";
		}
	    myMsg += "\r\n";
		//End of list
		myMsg += ":" + server.getHost() + " 366 " + client.getNickname() + " " + this->getName() + " :End of /NAMES list.\r\n";
		send(client.getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
	}
}

//Channel command to kick
void Channel::Kick(std::string nick, Server& server, std::string reason, bool leave, Client& sender) {
	int clientPos = this->findClientJoin(nick);
	if (clientPos >= 0) {

		std::string myMsg;
		if (!reason.empty()) {
		    myMsg = ":" + sender.getNickname() + " KICK " + this->getName() + " " + nick + " :" + reason + "\r\n";
		} else {
		    myMsg = ":" + sender.getNickname() + " KICK " + this->getName() + " " + nick + "\r\n";
		}
		if (leave == false)
			this->Broadcast(sender, myMsg, BRCST_KICK, server);
		this->_joinedList.erase(_joinedList.begin() + clientPos);
	}
	int opPos = this->findClientOp(nick);
	if (opPos != -1)
		this->_opList.erase(_opList.begin() + opPos);
}

//Channel command to add invite
void Channel::Invite(Client& client, Server& server) {
	int clientPos = this->findClientInvite(client);
	if (clientPos == -1)
		this->_inviteList.push_back(&client);
	(void)server;
}

//Channel command to remove invite
void Channel::DeInvite(Client& client, Server& server) {
	int clientPos = this->findClientInvite(client);
	if (clientPos >= 0)
		this->_inviteList.erase(_inviteList.begin() + clientPos);
	(void)server;
}

//Channel command to add operator
void Channel::Op(Client& client, Server&server, Client& sender) {
	int clientPos = this->findClientOp(client);
	if (clientPos == -1) {
		this->_opList.push_back(&client);
		std::string myMsg = ":" + sender.getHostname() + " MODE " + this->getName() + " +o " + client.getNickname() + "\r\n";
		this->Broadcast(client, myMsg, BRCST_OP, server);
	}
}

//Channel command to remove operator
void Channel::DeOp(Client& client, Server& server, Client& sender) {
	int clientPos = this->findClientOp(client);
	if (clientPos != -1) {
		this->_opList.erase(_opList.begin() + clientPos);
		std::string myMsg = ":" + sender.getHostname() + " MODE " + this->getName() + " -o " + client.getNickname() + "\r\n";
		this->Broadcast(client, myMsg, BRCST_DEOP, server);
	}
}

//Channel command to set topic
void Channel::Topic(std::string topic, Server& server) {
	if (topic.length() < 256)
	{
		this->_topic = topic;
		//std::string myMsg = ":" + server.getHost() + " 332 " + this->getName() + " :" + this->_topic + "\r\n";
		//this->Broadcast(*_joinedList[0] , myMsg, BRCST_TOPIC, server);
	}
	(void)server;
}

//Channel commands
void Channel::Mode(Client& sender, std::string option, Server& server) {
	int senderPos = this->findClientOp(sender);
	std::istringstream iss(option);
	std::string opt, target, channel;
		iss >> channel >> opt >> target;
	if (opt == "+l") {
		int maxValue = std::atoi(target.c_str());
		if (maxValue > 0) {
			this->_maxClient = maxValue;
		}
		std::cout << "Max client set to " << this->_maxClient << std::endl;
	}
	else if (opt == "-l") {
		this->_maxClient = 0;
		std::cout << "Max client remove" << std::endl;
	}
	else if (opt == "+t") {
		this->_needOpTopic = true;
		std::cout << "Need op to set topic" << std::endl;
	}
	else if (opt == "-t") {
		this->_needOpTopic = false;
		std::cout << "No need op to set topic" << std::endl;
	}
	else if (opt == "+i") {
		this->_needInvite = true;
		std::cout << "Need invite to join channel" << std::endl;
	}
	else if (opt == "-i") {
		this->_needInvite = false;
		std::cout << "No need invite to join channel" << std::endl;
	}
	else if (opt == "+k") {
		this->_needPassword = true;
		this->setPassword(target);
		std::cout << "Need password to join channel, the pass is '" << this->getPassword() << "'" << std::endl;
	}
	else if (opt == "-k") {
		this->_needPassword = false;
		this->setPassword("");
		std::cout << "No need password to join channel" << std::endl;
	}
	else if (!target.empty() && opt != "+o" && opt != "-o") {
		int targetPos = this->findClientJoin(target);
		if (targetPos == -1)
			return; //throw() no client find to aply mode
		Client *target = this->_joinedList[targetPos];
		if (senderPos == -1)
			return ;//throwRFCException(ERR_NOTONCHANNEL, this->getName()); ?? // PAS OP
		if (opt == "+o")
			this->Op(*target, server, sender); 
		else if (opt == "-o")
			this->DeOp(*target, server, sender);
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
