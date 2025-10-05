/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:52:28 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/05 14:55:27 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Server/Server.hpp"
#include "Errors/Exception.hpp"

bool	isOnlyDigit(std::string str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
}

void	sigintHandler(int signal) {
	if (signal == SIGINT)
		std::cout << "\n🛑 Server has been stopped by CTRL+C" << std::endl;
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
		signal(SIGINT, sigintHandler);
		server.start();
	}
	catch (ParsingException& e){
		std::cout << e.what() << std::endl;
	}
}
