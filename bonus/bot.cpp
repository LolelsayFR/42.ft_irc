/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 15:07:51 by arthur            #+#    #+#             */
/*   Updated: 2025/10/08 16:16:12 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <csignal>

int clientSocket = -1;
bool isRunning = true;

void handle_sigint(int) {
	std::cout << "\nCaught SIGINT, closing socket..." << std::endl;
	if (clientSocket >= 0) {
		std::string quitMessage = "QUIT\r\n";
		send(clientSocket, quitMessage.c_str(), quitMessage.length(), 0);
		close(clientSocket);
		clientSocket = -1;
	}
	isRunning = false;
}

int main(int argc, char** argv) {
	std::signal(SIGINT, handle_sigint);

	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <IP_ADDRESS> <PORT> <PASSWORD>" << std::endl;
		return 1;
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {
		std::cerr << "Socket creation error" << std::endl;
		return 1;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(std::atoi(argv[2]));

	if (inet_pton(AF_INET, argv[1], &serverAddress.sin_addr) <= 0) {
		std::cerr << "Invalid address/ Address not supported" << std::endl;
		close(clientSocket);
		return 1;
	}

	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Connection Failed" << std::endl;
		close(clientSocket);
		return 1;
	}

	std::cout << "Connected to server " << argv[1] << ":" << argv[2] << std::endl;

	std::string cap  = "CAP LS\r\n";
	std::string pass = "PASS " + std::string(argv[3]) + "\r\n";
	std::string nick = "NICK bot\r\n";
	std::string user = "USER bot " + std::string(argv[1]) + " :bot\r\n";

	send(clientSocket, cap.c_str(), cap.length(), 0);
	send(clientSocket, pass.c_str(), pass.length(), 0);
	send(clientSocket, nick.c_str(), nick.length(), 0);
	send(clientSocket, user.c_str(), user.length(), 0);

	std::string join = "JOIN #botchannel\r\n";
	send(clientSocket, join.c_str(), join.length(), 0);

	while (true) {
		if (!isRunning)
			break;
		char buffer[4096] = {0};
		int valread = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
		if (valread <= 0) {
			std::cout << "Disconnected from server" << std::endl;
			break;
		}

		buffer[valread] = '\0';
		std::string message(buffer);
		std::cout << "Server: " << message << std::endl;

		size_t pos = message.find("PRIVMSG");
		if (pos != std::string::npos) {
			std::string target, content;
			size_t start = pos + 8;
			size_t end = message.find(" :", start);
			if (end != std::string::npos) {
				target  = message.substr(start, end - start);
				content = message.substr(end + 2);
			}

			while (!target.empty() && target[0] == ' ')
				target.erase(0, 1);

			if (content.find("quoi") != std::string::npos) {
				std::string response = "PRIVMSG " + target + " :feur\r\n";
				send(clientSocket, response.c_str(), response.length(), 0);
				std::cout << "Sent: " << response;
			}
		}
	}

	if (clientSocket >= 0) {
		std::string quitMessage = "QUIT :Normal exit\r\n";
		send(clientSocket, quitMessage.c_str(), quitMessage.length(), 0);
		close(clientSocket);
	}

	return 0;
}
