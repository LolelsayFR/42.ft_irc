/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artgirar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:11:12 by artgirar          #+#    #+#             */
/*   Updated: 2025/09/30 11:44:20 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

void	start(void)
{
	// specifying the serv address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Configuration du socket...
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	std::vector<struct pollfd> fds;

    // Ajouter le socket serveur
    pollfd server_poll;
    server_poll.fd = server_fd;
    server_poll.events = POLLIN;
    fds.push_back(server_poll);

    // binding socket.
    bind(server_fd, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));

    // listening to the assigned socket
    listen(server_fd, 5);

	while (true) {
		int ret = poll(fds.data(), fds.size(), -1); // -1 = attente infinie

		if (ret < 0) break;

		for (size_t i = 0; i < fds.size(); i++) {
			if (fds[i].revents & POLLIN) {
				if (fds[i].fd == server_fd) {
					// Nouvelle connexion
					int client = accept(server_fd, nullptr, nullptr);
					pollfd client_poll;
					client_poll.fd = client;
					client_poll.events = POLLIN;
					fds.push_back(client_poll);

					std::cout << "user connexion" << std::endl;
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

int	main(void)
{
		start();
}
