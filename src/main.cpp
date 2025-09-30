/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:52:28 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 13:53:47 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

#include "../includes/Errors/Lang.hpp"
#include "../includes/Errors/Exception.hpp"
#include "../includes/Server/Server.hpp"

int main(int argc, char const *argv[])
{
	try {
		if(argc != 3)
			throw (ArgsNumberErrorException());
		int port = std::atoi(argv[1]);
		std::string password = argv[2];
		if (port == 0)
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
