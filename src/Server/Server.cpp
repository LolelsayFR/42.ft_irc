/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/01 14:13:00 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Client/Client.hpp"
#include "Server/Server.hpp"
#include "Errors/Exception.hpp"

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

bool	checkPass(int clientSocket, std::string password);

void parseMessage(Client &client, const std::string &msg) {
	std::istringstream iss(msg);
	std::string command;
	iss >> command;

	if (command == "NICK") {
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
	char buffer[1024];
	int clientSocket;
	std::cout << "Port : " << this->_port << " Password : " << this->_password << std::endl;
	// specifying the serv address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(this->_port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Configuration du socket...
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	// binding socket.
	if (bind(server_fd, (struct sockaddr*)&serverAddress,
		sizeof(serverAddress)) == -1)
			throw PortErrorException();

	std::vector<struct pollfd> fds;

	// Ajouter le socket serveur
	pollfd server_poll;
	server_poll.fd = server_fd;
	server_poll.events = POLLIN;
	fds.push_back(server_poll);

	// listening to the assigned socket
	listen(server_fd, 5);

	while (true) {
		int ret = poll(fds.data(), fds.size(), -1); // -1 = attente infinie

		if (ret < 0) break;

		for (size_t i = 0; i < fds.size(); i++) {
			if (fds[i].revents & POLLIN) {
				if (fds[i].fd == server_fd) {
					// Nouvelle connexion
					clientSocket = accept(server_fd, NULL, NULL);
					FdOutBuf		buf(clientSocket);
					std::ostream	clientStream(&buf);

					std::cout << "User try to connect..." << std::endl;
					// if (checkPass(clientSocket, _password) == false)
					// {
					// 	close(clientSocket);
					// 	std::cout << "User failed to connect" << std::endl;
					// }
					//else
					//{
						pollfd client_poll;
						client_poll.fd = clientSocket;
						client_poll.events = POLLIN;
						fds.push_back(client_poll);
						_clientList.push_back(new Client(clientSocket));
						std::cout << "User connected" << std::endl;
					//}

				} else {
					// Données d'un client existant
					int n = read(fds[i].fd, buffer, sizeof(buffer));
					if (n <= 0) {
						std::cout << "User disconnected" << std::endl;
						close(fds[i].fd);
						delete findClientByFd(_clientList, fds[i].fd);

						fds.erase(fds.begin() + i);
						continue;
					}
					Client *client = findClientByFd(_clientList, fds[i].fd);
					if (client)
					{
						client->appendBuffer(buffer, n);

						while (client->hasMessage())
						{
							std::string msg = client->popMessage();
							try {
								parseMessage(*client, msg);
								Server::isAvailable(*client);
							}
							catch (RFCException &e) {
								std::cerr << e.what() << std::endl;
								close(fds[i].fd);

								fds.erase(fds.begin() + i);
								break;
							}
						}
					}

				}
			}
		}
	}
	close(server_fd);
}

std::vector<Client*>::iterator Server::isAvailable(Client& client) {
	std::vector<Client*>::iterator	it = this->_clientList.begin();
	std::vector<Client*>::iterator	end = this->_clientList.end();
	while(it != end) {
		if (*it != &client && (static_cast<Client*>(*it)->getNickname() == client.getNickname() || static_cast<Client*>(*it)->getUsername() == client.getUsername()))
			throwRFCException(ERR_ALREADYREGISTRED);
		it++;
	}
	return (end);
}

