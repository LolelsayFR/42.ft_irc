/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkPass.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artgirar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:51:48 by artgirar          #+#    #+#             */
/*   Updated: 2025/10/01 16:00:02 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

bool	checkPass(int clientSocket, std::string password)
{
	size_t pos = 0;
	char buffer[1024];
	std::string	conv;
	while (int n = recv(clientSocket, buffer, 1024, 0))
	{
		buffer[n] = '\0';
		std::cout.write(buffer, n);
		conv = buffer;
		pos = conv.find("PASS ");
		if (pos != std::string::npos)
			break ;
	}
	if (pos == std::string::npos)
		return (false);
	if (conv.substr(pos + 5, password.size()) == password)
		if (conv[pos + 5 + password.size() + 1] == '\n')
			return (true);
	return (false);
}
