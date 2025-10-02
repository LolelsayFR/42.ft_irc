/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/02 10:52:43 by artgirar         ###   ########.fr       */
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
Server::Server(int port, std::string password) : _port(port), _password(password) {}

// Default destructor
Server::~Server(void) {
}


/* ************************************************************************** */
/* All operator overload */
/* ************************************************************************** */

//Ostream insertion operator
std::ostream& operator<<(std::ostream& o, Server& s) {
	Client* ptr;
	o << "/* Server View ********************************************************* */" << std::endl;
	o << "/\tPort : " << s.getPort() << "\n/\tPassword : " << s.getPassword() << std::endl;
	{
		std::vector<Client*> list = s.getClientList();
		std::vector<Client*>::iterator	it = list.begin();
		std::vector<Client*>::iterator	end = list.end();
		o << "/* Client connected **************************************************** */" << std::endl;
		for (int i = 0; it != end; i++, it++) {
			ptr = *it;
			{
			o 	<< "| n°" << std::setw(5) << i
				<< " | Username = " <<  std::setw(10) << ptr->getUsername()
				<< " | Nickname = " <<  std::setw(10) << ptr->getNickname()
				<< " |"  << std::endl;	
			}	
		}
		if (list.empty())
			o << "/\tEmpty.." << std::endl;
		o << "/* ********************************************************************* */" << std::endl;
	}
	{
		std::vector<Channel*> list = s.getChannelList();
		std::vector<Channel*>::iterator	it = list.begin();
		std::vector<Channel*>::iterator	end = list.end();
		for (int i = 0; it != end; i++, it++) {
			o << *(static_cast<Channel*>(*it));
			if (list.empty())
				o << "\tEmpty.." << std::endl;
		}
	}
	return (o);
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


//Channel JoinList getter
const std::vector<Client*>&	Server::getClientList(void) const {
	return (this->_clientList);
}

//Channel OpList getter
const std::vector<Channel*>&	Server::getChannelList(void) const {
	return (this->_channelList);
}

//Create new channel
void Server::makeChannel(std::string name) {
	int channelPos = this->findChannel(name);
	if (channelPos == -1)
		this->_channelList.push_back(new Channel(name));		
}


//Client id in join vector list by ref
void welcomeUser(Client *client)
{
	if (!client->getWelcomeSent())
	{
		std::string welcomeMsg = ": ""001 " + client->getNickname() + " :Welcome to the IRC Network, " + client->getNickname() + "\r\n";
		std::string infoMsg = ": ""002 " + client->getNickname() + " :Your host is localhost, running version 1.0\r\n";
		std::string yourHostMsg = ": ""003 " + client->getNickname() + " :This server was created today\r\n";
		std::string myInfoMsg = ": ""004 " + client->getNickname() + " localhost 1.0 o o\r\n";
		send(client->getFd(), welcomeMsg.c_str(), welcomeMsg.size(), MSG_NOSIGNAL);
		send(client->getFd(), infoMsg.c_str(), infoMsg.size(), MSG_NOSIGNAL);
		send(client->getFd(), yourHostMsg.c_str(), yourHostMsg.size(), MSG_NOSIGNAL);
		send(client->getFd(), myInfoMsg.c_str(), myInfoMsg.size(), MSG_NOSIGNAL);
		client->setWelcomeSent(true);
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
	for (size_t j = 0; j < _clientList.size(); j++) {
		if (_clientList[j] == target) {
			_clientList.erase(_clientList.begin() + j);\
				break;
		}
	}
	delete target;
	close(fds[i].fd);
	fds.erase(fds.begin() + i);
}

void	addNewSocket(std::vector<struct pollfd> &fds , int server_fd)
{
	pollfd server_poll;
	server_poll.fd = server_fd;
	server_poll.events = POLLIN;
	server_poll.revents = 0;
	fds.push_back(server_poll);
}

void Server::parseMessage(Client &client, const std::string &msg) {
	std::istringstream iss(msg);
	std::string command;
	iss >> command;

	if (command == "PING")
	{
		std::string server;
		iss >> server;
		std::cout << "Received PING from " << server << std::endl;
		std::string pongResponse = "PONG :" + server + "\r\n";
		send(client.getFd(), pongResponse.c_str(), pongResponse.size(), MSG_NOSIGNAL);
		std::cout << "Responded to PING with PONG" << std::endl;
	}
	if (command == "PASS") {
		std::string pass;
		iss >> pass;
		if (pass.empty())
			throw ClientPasswordException();
		if (pass != _password)
			throw ClientPasswordException();
		std::cout << "Password Correct" << std::endl;
	}
	else if (command == "NICK") {
		std::string nick;
		iss >> nick;
		if (nick.empty())
			throwRFCException(ERR_ALREADYREGISTRED);
		client.setNickname(nick);
		std::cout << "Nickname set to " << nick << " for fd " << client.getFd() << std::endl;
	}
	else if (command == "USER") {
		std::string username, host, server, realname;
		iss >> username >> host >> server;
		std::getline(iss, realname);
		if (!realname.empty() && realname[0] == ':')
			realname.erase(0, 1);
		client.setUsername(username);
		std::cout << "Username set to " << username
				  << " for fd " << client.getFd() << std::endl;
	}
	else {
		std::cout << "Unknown command: " << command << std::endl;
	}
}

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
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
		close(server_fd);
		throw SetOptionSocketErrorException();
	}

	// binding socket.
	if (bind(server_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		close(server_fd);
		throw PortErrorException();
	}

	std::vector<struct pollfd> fds;

	// Ajouter le socket serveur
	addNewSocket(fds, server_fd);

	// listening to the assigned socket
	if (listen(server_fd, 5) == -1) {
		close(server_fd);
		fds.erase(fds.begin());
		throw ListeningErrorException();
	}

	std::cout << "Server is on" << std::endl;

	while (true) {
		int ret = poll(fds.data(), fds.size(), -1); // -1 = attente infinie

		if (ret < 0)
			break;

		for (size_t i = 0; i < fds.size(); i++) {
			if (fds[i].revents != 0) {
				if (fds[i].fd == server_fd) {
					// Nouvelle connexion
					int clientSocket = accept(server_fd, NULL, NULL);
					if (clientSocket == -1)
						throw SocketErrorException();
					FdOutBuf		buf(clientSocket);
					std::ostream	clientStream(&buf);

					std::cout << "User try to connect..." << std::endl;

					addNewSocket(fds, clientSocket);
					_clientList.push_back(new Client(clientSocket));
				} else {
					// Données d'un client existant
					int n = recv(fds[i].fd, buffer, 1024, 0);
					if (n > 0) {
					    std::string msg(buffer, n);
					    std::cout << "Received: " << msg << std::endl;
					}
					if (n <= 0) {
						std::cout << "User disconnected" << std::endl;
						destroyOneClient(fds, i);
						std::cout << *this << std::endl;
						continue ;
					}
					// Traitement des donnees
					Client *client = findClientByFd(_clientList, fds[i].fd);
					if (client) {

						client->appendBuffer(buffer, n);
						while (client->hasMessage()) {
							std::cout << "Message received from client: ";
							std::string msg = client->popMessage();
							try {
								parseMessage(*client, msg);
								Server::isAvailable(*client);
								client->checkRegistration();
								if (client->isRegistered()) {
									welcomeUser(client);
								}
							}
							catch (RFCException &e) {
								destroyOneClient(fds, i);
								std::cerr << e.what() << std::endl;
								break ;
							}
							catch (ClientPasswordException & e) {
								destroyOneClient(fds, i);
								std::cerr << e.what() << std::endl;
								break ;
							}
						}
					}
				}
			}
		}
	}
}

std::vector<Client*>::iterator Server::isAvailable(Client& client) {
	std::vector<Client*>::iterator	it = this->_clientList.begin();
	std::vector<Client*>::iterator	end = this->_clientList.end();
	while(it != end) {
		if (*it != &client && (static_cast<Client*>(*it)->getNickname() == client.getNickname() ||
				static_cast<Client*>(*it)->getUsername() == client.getUsername()))
			throwRFCException(ERR_ALREADYREGISTRED);
		it++;
	}
	return (end);
}
