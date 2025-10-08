/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/08 13:42:32 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Client/Client.hpp"
#include "Channel/Channel.hpp"
#include "Server/Server.hpp"
#include "Errors/Exception.hpp"

/* ************************************************************************** */
/* All constructors and the destructor */
/* ************************************************************************** */

// Assignation constructor
Server::Server(int port, std::string password) : _port(port), _password(password) {
	char hostname[256];
	gethostname(hostname, sizeof(hostname));
	this->_hostName = hostname;
}

// Default destructor
Server::~Server(void) {
	for (int i = this->_clientList.size(); i > 0; i--) {
		this->destroyOneClient(this->getFds(), i);
	}
	for (int i = this->_setupList.size(); i > 0; i--) {
		delete this->_setupList[i - 1];
	}
	std::vector<Channel*>::iterator	it = this->_channelList.begin();
	std::vector<Channel*>::iterator	end = this->_channelList.end();
	for(int i = 0; it != end; i++) {
		delete	static_cast<Channel*>(*it);
		it++;
	}
	this->_channelList.clear();
	this->_clientList.clear();
	this->_setupList.clear();
}

/* ************************************************************************** */
/* All members functions */
/* ************************************************************************** */

int Server::getPort(void) const {
	return (this->_port);
}

std::string Server::getPassword(void) const {
	return (this->_password);
}

std::string Server::getHost(void) const {
	return (this->_hostName);
}

//Client pollfds list return
std::vector<struct pollfd>& Server::getFds(void) {
	return (this->_fds);
}
//Channel JoinList getter
const std::vector<Client*>&	Server::getClientList(void) const {
	return (this->_clientList);
}

//Channel JoinList getter
const std::vector<Client*>&	Server::getSetupList(void) const {
	return (this->_setupList);
}

//Channel OpList getter
const std::vector<Channel*>&	Server::getChannelList(void) const {
	return (this->_channelList);
}

//Create new channel
Channel* Server::makeChannel(std::string name, std::string pass) {
	int channelPos = this->findChannel(name);
	if (channelPos == -1) {
		this->_channelList.push_back(new Channel(name));
		if (!pass.empty()) {
			this->_channelList.back()->setPassword(pass);
			this->_channelList.back()->setNeedPassword(true);
		}
	}
	else
		return (this->_channelList[channelPos]);
	return (this->_channelList.back());
}


//Client id in join vector list by ref
void Server::welcomeUser(Client *client)
{
	if (!client->getWelcomeSent())
	{
		std::string welcomeMsg = ": ""001 " + client->getNickname() + " :" + WLCM_MESSAGE + ": your nick is " + client->getNickname() + "\r\n";
		welcomeMsg += ": 002 " + client->getNickname() + " :Your host is " + this->_hostName + ", running version 1.0\r\n";
		welcomeMsg += ": 003 " + client->getNickname() + " :This server was created today\r\n";
		welcomeMsg += ": 004 " + client->getNickname() + " " + this->_hostName + " " + client->getHostname() + "\r\n";
		send(client->getUid(), welcomeMsg.c_str(), welcomeMsg.size(), MSG_NOSIGNAL);
		client->setWelcomeSent(true);
		int setupPos = this->findClientSetup(client->getUid());
		this->_setupList.erase(this->_setupList.begin() + setupPos);
		this->_clientList.push_back(client);
		std::cout << WHI << "USER SETUP OK" << RES << std::endl;
		//Broadcast welcome message to all clients
		std::vector<Client*>::iterator	it = this->_clientList.begin();
		std::vector<Client*>::iterator	end = this->_clientList.end();
		std::string broadcastMsg = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " JOIN :" + WLCM_MESSAGE + "\r\n";
		for (; it != end; it++) {
			if (static_cast<Client*>(*it)->getNickname() != client->getNickname())
				send(static_cast<Client*>(*it)->getUid(), broadcastMsg.c_str(), broadcastMsg.length(), MSG_NOSIGNAL);
		}

	}
}

//Client id in join vector list
int Server::findClient(Client& client) {
	std::vector<Client*>::iterator	it = this->_clientList.begin();
	std::vector<Client*>::iterator	end = this->_clientList.end();
	for(int i = 0; it != end; i++) {
		if (*it == &client)
			return (i);
		it++;
	}
	return (-1);
}

//Channel id in join vector list by name
int Server::findClient(std::string name) {
	std::vector<Client*>::iterator	it = this->_clientList.begin();
	std::vector<Client*>::iterator	end = this->_clientList.end();
	for(int i = 0; it != end; i++) {
		if (static_cast<Client*>(*it)->getUsername() == name)
			return (i);
		it++;
	}
	return (-1);
}

//Channel id in join vector list by nick
int Server::findClientByNick(std::string nickname) {
	std::vector<Client*>::iterator	it = this->_clientList.begin();
	std::vector<Client*>::iterator	end = this->_clientList.end();
	for(int i = 0; it != end; i++) {
		if (static_cast<Client*>(*it)->getNickname() == nickname) {
			return (i);
		}
		it++;
	}
	return (-1);
}

//Find client in setup list by fd
int Server::findClientSetup(int fd) {
	std::vector<Client*>::iterator	it = this->_setupList.begin();
	std::vector<Client*>::iterator	end = this->_setupList.end();
	for(int i = 0; it != end; i++) {
		if (static_cast<Client*>(*it)->getUid() == fd) {
			return (i);
		}
		it++;
	}
	return (-1);
}

//Find client by fd
int Server::findClient(int fd) {
	std::vector<Client*>::iterator	it = this->_clientList.begin();
	std::vector<Client*>::iterator	end = this->_clientList.end();
	for(int i = 0; it != end; i++) {
		if (static_cast<Client*>(*it)->getUid() == fd) {
			return (i);
		}
		it++;
	}
	return (-1);
}

//Channel id in join vector list by ref
int Server::findChannel(Channel& channel) {
	std::vector<Channel*>::iterator	it = this->_channelList.begin();
	std::vector<Channel*>::iterator	end = this->_channelList.end();
	for(int i = 0; it != end; i++) {
		if (*it == &channel)
			return (i);
		it++;
	}
	return (-1);
}

//Channel id in join vector list by name
int Server::findChannel(std::string name) {
	std::vector<Channel*>::iterator	it = this->_channelList.begin();
	std::vector<Channel*>::iterator	end = this->_channelList.end();
	for(int i = 0; it != end; i++) {
		if (static_cast<Channel*>(*it)->getName() == name)
			return (i);
		it++;
	}
	return (-1);
}

void	Server::destroyOneClient(std::vector<struct pollfd> &fds , int i)
{
	Client* target = findClientByFd(_clientList, fds[i].fd);
	Channel* channel;
	//destroy client in all channel
	std::vector<Channel*>::iterator	it = this->_channelList.begin();
	std::vector<Channel*>::iterator	end = this->_channelList.end();
	for(int i = 0; it != end; i++) {
		channel = static_cast<Channel*>(*it);
		if (channel->findClientJoin(*target) != -1)
			channel->Kick(target->getNickname(), "", true, *target);
		if (channel->findClientInvite(*target) != -1)
			channel->DeInvite(*target, *this);
		if (channel->findClientOp(*target) != -1)
			channel->DeOp(*target, *target);
		it++;
	}
	//destroy client in server base class
	for (size_t j = 0; j < _clientList.size(); j++) {
		if (_clientList[j] == target) {
			_clientList.erase(_clientList.begin() + j);
				break;
		}
	}
	delete target;
	close(fds[i].fd);
	fds.erase(fds.begin() + i);
}

void	Server::destroyOneWaiting(std::vector<struct pollfd> &fds , int i)
{
	Client* target = findClientByFd(_setupList, fds[i].fd);
	Channel* channel;
	//destroy client in all channel
	std::vector<Channel*>::iterator	it = this->_channelList.begin();
	std::vector<Channel*>::iterator	end = this->_channelList.end();
	for(int i = 0; it != end; i++) {
		channel = static_cast<Channel*>(*it);
		if (channel->findClientJoin(*target) != -1)
			channel->Kick(target->getNickname(), "", true, *target);
		if (channel->findClientInvite(*target) != -1)
			channel->DeInvite(*target, *this);
		if (channel->findClientOp(*target) != -1)
			channel->DeOp(*target, *target);
		it++;
	}
	//destroy client in server base class
	for (size_t j = 0; j < _setupList.size(); j++) {
		if (_setupList[j] == target) {
			_setupList.erase(_setupList.begin() + j);
				break;
		}
	}
	delete target;
	close(fds[i].fd);
	fds.erase(fds.begin() + i);
}

void	addNewSocket(std::vector<struct pollfd> &fds , int socketFD)
{
	pollfd server_poll;
	server_poll.fd = socketFD;
	server_poll.events = POLLIN;
	server_poll.revents = 0;
	fds.push_back(server_poll);
}

void Server::parseMessage(Client &client, const std::string &msg) {
	std::istringstream iss(msg);
	std::string command;
	iss >> command;
	if (command == "NICK") {
		std::string nick;
		iss >> nick;
		this->setClientNick(client, nick);
	}
	else if (command == "USER") {
		std::string username, hostname, servername, realname;
		iss >> username >> hostname >> servername;
		std::getline(iss, realname);
		client.setUsername(username);
		if (hostname.empty())
			hostname = "unknown";
		client.setHostname(hostname);
		if (realname.length() >= 2)
			client.setRealname(realname.c_str() + 2);
		else
			client.setRealname(username);
		std::cout << "Username set to " << username << " for fd " << client.getUid() << std::endl;
	}
	else if (command == "PASS") {
		std::string pass;
		iss >> pass;
		if (pass.empty())
			throw ClientPasswordException();
		if (pass != _password)
			throw ClientPasswordException();
		std::cout << "Password Correct" << std::endl;
	}
	else if (command == "PING")
	{
		std::string server;
		iss >> server;
		std::string pongResponse = "PONG :" + server + "\r\n";
		send(client.getUid(), pongResponse.c_str(), pongResponse.size(), MSG_NOSIGNAL);
	}
	else if (this->findClientSetup(client.getUid()) != -1)
		return;
	else if (command == "INVITE") {
		std::string targetNick, channelName;
		iss >> targetNick >> channelName;
		int targetPos = this->findClientByNick(targetNick);
		if (targetPos == -1)
			throwRFCException(ERR_NOSUCHNICK, targetNick, client.getNickname());
		int channelPos = this->findChannel(channelName);
		if (channelPos == -1)
			throwRFCException(ERR_NOSUCHNICK, channelName, client.getNickname());
		this->_channelList[channelPos]->Invite(*this->_clientList[targetPos], client);

	}
	else if (command == "JOIN") {
		std::string arg;
		getline(iss, arg);
		this->linkClientToChannel(client, arg);
	}
	else if (command == "PRIVMSG") {
		this->privMsgSend(client, msg);
	}
	else if (command == "PART") {
		this->clientLeaveChannel(client, msg);
	}
	else if (command == "KICK") {
		this->clientLeaveChannel(client, msg);
		std::string channel, targetNick, reason;
		iss >> channel >> targetNick;
		getline(iss, reason);
		int channelPos = this->findChannel(channel);
		if (channelPos == -1)
			throwRFCException(ERR_NOSUCHNICK, channel, client.getNickname());
		int targetPos = this->_channelList[channelPos]->findClientJoin(targetNick);
		if (this->_channelList[channelPos]->findClientOp(client) == -1)
			throwRFCException(ERR_CHANOPRIVSNEEDED, targetNick, client.getNickname());
		else if (targetPos == -1)
			throwRFCException(ERR_NOSUCHNICK, targetNick, client.getNickname());
		this->_channelList[channelPos]->Kick(targetNick, reason.c_str() + 2, false, client);
	}
	else if (command == "MODE") {
		std::string cmd;
		std::getline(iss, cmd);
		this->sendModeChannel(client, cmd.c_str() + 1);
	}
	else if (command == "TOPIC")
	{
		std::string channel, topic;
		iss >> channel;
		std::getline(iss, topic);

		int channelPos = this->findChannel(channel);
		if (channelPos == -1)
			throwRFCException(ERR_NOSUCHNICK, channel, client.getNickname());
		if (this->_channelList[channelPos]->findClientJoin(client) == -1)
			throwRFCException(ERR_NOTONCHANNEL, channel, client.getNickname());
		if (!topic.empty())
			this->_channelList[channelPos]->Topic(topic.c_str() + 2, *this, client);
	}
	else if (command == "QUIT")
		close(client.getUid());
	else {
		std::cout << "Unknown command: " << command << std::endl << *this;
	}
}

void Server::linkClientToChannel(Client& client, std::string& arg) {
	std::string work, name, pass;
	int pos = arg.rfind(",");
	while (pos > 0 && pos < (int)arg.length()) {
		work = arg.substr(pos + 1, arg.length() - pos);
		std::istringstream iss(work);
		iss >> name >> pass;
		arg.erase(pos, arg.length() - pos);
		pos = arg.rfind(",");

		makeChannel(name, pass)->Join(client, *this, pass);
	}
	work = arg.substr(pos + 1, arg.length() - pos);
	std::istringstream iss(work);
	iss >> name >> pass;
	std::cout << "Joining channel: " << name << " with pass: " << pass << std::endl;
	makeChannel(name, pass)->Join(client, *this, pass);
}

void Server::setClientNick(Client& client, std::string& nick) {

		if (nick.empty())
			throwRFCException(ERR_ALREADYREGISTRED, nick, client.getNickname());
		if (this->findClientByNick(nick) == -1) {
			std::string myMsg = ":" + client.getNickname() + " NICK " + " :" + nick + "\r\n";
			send(client.getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
			client.setNickname(nick);
		}
		else
			return ;
			//EXCEPTION NICKNAME ALREADY USE
		std::cout << "Nickname set to " << nick << " for fd " << client.getUid() << std::endl;
}

void Server::privMsgSend(Client& client, const std::string& arg) {
	int separatorPos = arg.find(":");
	std::string	msg(arg.substr(separatorPos + 1)), dest(arg.substr(8, separatorPos - 9));
	if (dest[0] == '#' || dest[0] == '&' || dest[0] == '+' || dest[0] == '!') {
		int Pos = this->findChannel(dest);
		if (Pos == -1)
			throwRFCException(ERR_NOSUCHNICK, dest, client.getNickname());
		else
			this->_channelList[Pos]->Broadcast(client, msg, BRCST_PRVMSG);
	}
	else {
		int Pos = this->findClientByNick(dest);
		if (Pos == -1)
		{
			throwRFCException(ERR_NOSUCHNICK, dest, client.getNickname());
		}
		else
			this->_clientList[Pos]->receptMessage(client, msg);
	}
}

void Server::sendModeChannel(Client& client, const std::string& arg) {
	std::istringstream iss(arg);
	std::string target;
		iss >> target;
	if (arg[0] == '#' || arg[0] == '&' || arg[0] == '+' || arg[0] == '!') {
		int Pos = this->findChannel(target);
		if (Pos == -1)
			throwRFCException(ERR_NOSUCHNICK, arg.substr(1, arg.find(" ") - 1), client.getNickname());
		else
			this->_channelList[Pos]->Mode(client, arg);
	}
}

void Server::clientLeaveChannel(Client& client, const std::string& arg) {
	int separatorPos = arg.find(":");
	std::string dest;
	if (separatorPos > 0 && separatorPos < (int)arg.length())
		dest = std::string(arg.substr(5, separatorPos - 6));
	else
		dest = std::string(arg.substr(5));
	int Pos = this->findChannel(dest);
	if (Pos == -1)
		return	;
	else {
		if (this->_channelList[Pos]->findClientJoin(client) == -1)
			throwRFCException(ERR_NOTONCHANNEL, dest, client.getNickname());
		if (separatorPos > 0 && separatorPos < (int)arg.length()) {
			std::string	msg(arg.substr(separatorPos + 1));
			this->_channelList[Pos]->Broadcast(client, msg, BRCST_LEAVE_MSG);
		}
		else
			this->_channelList[Pos]->Broadcast(client, "", BRCST_LEAVE);
		this->_channelList[Pos]->Kick(client.getNickname(), "", true, client);

	}
}

/* ************************************************************************** */
/* Cliant Handler */
/* ************************************************************************** */

void Server::clientSetupHandler(int i, int n, char *buffer)
{
	if (this->findClientSetup(_fds[i].fd) != -1)
	{
		Client *client = findClientByFd(_setupList, _fds[i].fd);
		if (!client)
			return;
		client->appendBuffer(buffer, n);
		bool client_deleted = false;
		while (client->hasMessage()) {
			std::string msg = client->popMessage();
			try {
				int endMessages = 0;
				if (!msg.empty() && msg[msg.length() - 1] == '\r')
					endMessages = 1;
				parseMessage(*client, msg.erase(msg.length() - endMessages));
			}
			catch (AlreadyRegisteredException &e) {
				std::string errorMsg = ":" + this->_hostName + " " + e.what() + "\r\n";
				send(client->getUid(), errorMsg.c_str(), errorMsg.length(), MSG_NOSIGNAL);
				destroyOneWaiting(_fds, i);
				client_deleted = true;
				std::cerr << e.what() << std::endl;
				break ;
			}
			catch (RFCException &e) {
				std::string errorMsg = ":" + this->_hostName + " " + e.what() + "\r\n";
				send(client->getUid(), errorMsg.c_str(), errorMsg.length(), MSG_NOSIGNAL);
				std::cerr << e.what() << std::endl;
				break ;
			}
			catch (ClientPasswordException & e) {
				destroyOneWaiting(_fds, i);
				client_deleted = true;
				std::cerr << e.what() << std::endl;
				break ;
			}
		}
		if (client_deleted)
			return;
		Server::isAvailable(*client);
		client->checkRegistration();
		if (client->isRegistered()) {
			welcomeUser(client);
		}
	}
}

void Server::clientHandler(int i, int n, char *buffer)
{
	if(i < 0 || static_cast<size_t>(i) >= _fds.size())
		return;
	if (_fds[i].fd < 0)
		return;
	Client *client = findClientByFd(_clientList, _fds[i].fd);
	if (client && this->findClientSetup(_fds[i].fd) == -1) {
		client->appendBuffer(buffer, n);
		while (client->hasMessage()) {
			std::cout << "Message received from client: ";
			std::string msg = client->popMessage();
			try {
				int endMessages = 0;
				if (!msg.empty() && msg[msg.length() - 1] == '\r')
					endMessages = 1;
				parseMessage(*client, msg.erase(msg.length() - endMessages));
			}
			catch (AlreadyRegisteredException &e) {
				std::string errorMsg = ":" + this->_hostName + " " + e.what() + "\r\n";
				send(client->getUid(), errorMsg.c_str(), errorMsg.length(), MSG_NOSIGNAL);
				destroyOneClient(_fds, i);
				std::cerr << e.what() << std::endl;
				break ;
			}
			catch (RFCException &e) {
				std::string errorMsg = ":" + this->_hostName + " " + e.what() + "\r\n";
				send(client->getUid(), errorMsg.c_str(), errorMsg.length(), MSG_NOSIGNAL);
				std::cerr << e.what() << std::endl;
				break ;
			}
			catch (ClientPasswordException & e) {
				destroyOneClient(_fds, i);
				std::cerr << e.what() << std::endl;
				break ;
			}
			catch (std::exception & e) {
				std::cerr << "Unexpected error: " << e.what() << std::endl;
				break ;
			}
		}
	}
}

/* ************************************************************************** */
/* Server start and loop */
/* ************************************************************************** */

void Server::start(void){
	char buffer[4096];
	std::cout << *this << std::endl;
	// specifying the serv address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(this->_port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	// Configuration du socket...
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		throw SocketErrorException();
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		close(server_fd);
		throw SetOptionSocketErrorException();
	}
	// binding socket.
	if (bind(server_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		close(server_fd);
		throw PortErrorException();
	}
	// Ajouter le socket serveur
	addNewSocket(_fds, server_fd);
	// listening to the assigned socket
	if (listen(server_fd, 5) == -1) {
		close(server_fd);
		_fds.erase(_fds.begin());
		throw ListeningErrorException();
	}
	while (true) {
		int ret = poll(_fds.data(), _fds.size(), -1); // -1 = attente infinie
		if (ret < 0)
			break;

		for (size_t i = 0; i < _fds.size(); ) {
			if (_fds[i].revents != 0) {
				if (_fds[i].fd == server_fd) {
					int clientSocket = accept(server_fd, NULL, NULL);
					if (clientSocket == -1)
						break;
					std::cout << "User try to connect..." << std::endl;
					addNewSocket(_fds, clientSocket);
					_setupList.push_back(new Client(clientSocket));
					i++; // on avance, rien n’a été effacé
				} else {
					int n = recv(_fds[i].fd, buffer, 1024, 0);
					if (n <= 0) {
						std::cout << "User disconnected" << std::endl;
						if (findClientSetup(_fds[i].fd) != -1)
							destroyOneWaiting(_fds, i);
						else
						destroyOneClient(_fds, i);
						std::cout << *this << std::endl;
						continue;
					}
					this->clientSetupHandler(i, n, buffer);
					this->clientHandler(i, n, buffer);
					i++;
				}
			} else {
				i++;
			}
		}

	}
}

std::vector<Client*>::iterator Server::isAvailable(Client& client) {
	std::vector<Client*>::iterator	it = this->_clientList.begin();
	std::vector<Client*>::iterator	end = this->_clientList.end();
	while(it != end) {
		if (*it != &client && (static_cast<Client*>(*it)->getNickname() == client.getNickname() ||
				static_cast<Client*>(*it)->getNickname() == client.getNickname()))
			throwRFCException(ERR_ALREADYREGISTRED, client.getNickname(), client.getNickname());
		it++;
	}
	return (end);
}
