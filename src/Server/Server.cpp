/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:54:40 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 15:18:25 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
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
	std::cout << "Port : " << this->_port << " Password : " << this->_password << std::endl;
	// specifying the serv address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Configuration du socket...
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
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
					int client = accept(server_fd, NULL, NULL);
					pollfd client_poll;
					client_poll.fd = client;
					client_poll.events = POLLIN;
					fds.push_back(client_poll);

					std::cout << "user connexion with fd " << client << std::endl;
				} else {
						// Données d'un client existant
					char buffer[1024];
					int n = read(fds[i].fd, buffer, sizeof(buffer));
					if (n > 0)
						write(1, buffer, n);
					// Traiter les données...
					if (n == 0) {
						std::cout << "user disconnected" << std::endl;
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						i--;
					}
				}
			}
		}
	}
	close(server_fd);
}
