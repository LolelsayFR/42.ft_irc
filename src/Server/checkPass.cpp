/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkPass.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artgirar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:51:48 by artgirar          #+#    #+#             */
/*   Updated: 2025/10/01 09:51:49 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

bool	checkPass(int clientSocket, std::string password)
{
	char buffer[1024];
	int n = read(clientSocket, buffer, sizeof(buffer));

	std::cout.write(buffer, n);
	std::string	conv(buffer);

	size_t pos = conv.find("PASS");
	if (pos == std::string::npos)
		return (false);
	if (conv.substr(pos + 5, password.size()) == password)
		if (conv[pos + 5 + password.size() + 1] == '\n')
			return (true);
	return (false);
}
