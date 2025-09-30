/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkPass.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artgirar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:51:48 by artgirar          #+#    #+#             */
/*   Updated: 2025/09/30 19:22:51 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

bool	checkPass(/*Client client, */char *buf, std::string password)
{

	std::string	conv(buf);

	size_t pos = conv.find("PASS");
	if (pos == std::string::npos)
		return (false);
	if (conv.substr(pos + 5, password.size()) == password)
		if (conv[pos + 5 + password.size() + 1] == '\n')
			return (true);
	return (false);
}
