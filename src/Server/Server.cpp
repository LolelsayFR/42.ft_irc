/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/01 13:51:27 by artgirar         ###   ########.fr       */
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

void Server::start(void){
	char buffer[1024];
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

	std::cout << "Server is on" << std::endl;

	while (true) {
		int ret = poll(fds.data(), fds.size(), -1); // -1 = attente infinie

		if (ret < 0) break;

		for (size_t i = 0; i < fds.size(); i++) {
			if (fds[i].revents & POLLIN) {
				if (fds[i].fd == server_fd) {
					// Nouvelle connexion
					int clientSocket = accept(server_fd, NULL, NULL);
					FdOutBuf		buf(clientSocket);
					std::ostream	clientStream(&buf);

					std::cout << "User try to connect..." << std::endl;
					if (checkPass(clientSocket, _password) == false)
					{
						close(clientSocket);
						std::cout << "User failed to connect" << std::endl;
					}
					else
					{
						pollfd client_poll;
						client_poll.fd = clientSocket;
						client_poll.events = POLLIN;
						client_poll.revents = 0;
						fds.push_back(client_poll);
						std::cout << "User connected" << std::endl;
					}
					std::cout << fds.size() << std::endl;

				} else {
					// Données d'un client existant
					int n = read(fds[i].fd, buffer, sizeof(buffer));
					// Traiter les données...
					buffer[n] = '\0';
					write(1, buffer, n);
					if (n > 0 && _clientList[_clientList.size() - 1]->getNickname().empty())
					{
						_clientList[_clientList.size() - 1]->extractNickname(buffer);
						std::cout << "Username set to: " << _clientList[_clientList.size() - 1]->getUsername() << std::endl;
						std::cout << "Nickname set to: " << _clientList[_clientList.size() - 1]->getNickname() << std::endl;
					}
					else if (n > 0 && _clientList[_clientList.size() - 1]->getUsername().empty())
					{
						_clientList[_clientList.size() - 1]->extractUsername(buffer);
						std::cout << "Username set to: " << _clientList[_clientList.size() - 1]->getUsername() << std::endl;
						std::cout << "Nickname set to: " << _clientList[_clientList.size() - 1]->getNickname() << std::endl;
					}
					if (n == 0) {
						std::cout << "User disconnected" << std::endl;
						close(fds[i].fd);

						fds.erase(fds.begin() + i);
						//delete _clientList[i - 1];
						//_clientList.erase(_clientList.begin() + i - 1);
						i--;
					}
				}
			}
		}
	}
	close(server_fd);
}
