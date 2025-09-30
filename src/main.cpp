/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:52:28 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 11:06:53 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

#include "Lang.hpp"
#include "Exception.hpp"

int main(int argc, char const *argv[])
{
	try {		
		int port;
		(void) argc;
		port = std::atoi(argv[1]);
		if (port == 0)
			throw (PortErrorException());
		throw(PasswordErrorException());
	}
	catch (ParsingException& e){
		std::cout << e.what() << std::endl;
	}
	return (0);
}