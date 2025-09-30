/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:52:28 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 14:35:27 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

#include "Errors/Lang.hpp"
#include "Errors/Exception.hpp"
#include "Server/Server.hpp"

bool	isOnlyDigit(std::string str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
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
		server.start();
	}
	catch (ParsingException& e){
		std::cout << e.what() << std::endl;
	}
	return (0);
}