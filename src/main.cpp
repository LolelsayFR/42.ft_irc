/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:52:28 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/02 15:44:15 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Server/Server.hpp"
#include "Errors/Exception.hpp"

Server *serverPtr;

bool	isOnlyDigit(std::string str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
}

void	sigintHandler(int signal) {
	std::cout << " has been called with signal " << signal << std::endl;
	for (int i = serverPtr->getClientList().size(); i > 0; i--) {
		serverPtr->destroyOneClient(serverPtr->getFds(), i);
	}
	std::cout << "Server has been stopped" << std::endl;
}

int main(int argc, char const *argv[])
{
	try {
		if(argc != 3)
			throw (ArgsNumberErrorException());
		int port = std::atoi(argv[1]);
		std::string password = argv[2];
		if (port == 0 || !isOnlyDigit(argv[1]))
			throw (PortErrorException());
		if (password.empty())
			throw(PasswordErrorException());
		Server server(port, password);
		serverPtr = &server;
		signal(SIGINT, sigintHandler);
		server.start();
	}
	catch (ParsingException& e){
		std::cout << e.what() << std::endl;
	}
	return (0);
}
