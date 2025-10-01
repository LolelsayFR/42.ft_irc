/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:52:45 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/01 13:50:49 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"
#include "Client/Client.hpp"

class Client;

class Server {
private:
	const int			_port;
	const std::string	_password;
	std::vector<Client*>	_clientList;
	Server(void);
	Server(const Server& other);
	Server& operator=(const Server& other);
    //sockaddr_in			_serverAddress;
	//std::vector<Channel&>
public:
	//All constructor and destructor
	Server(int port, std::string password);
	~Server(void);
	//All other member functions
	void start(void);
	std::vector<Client*>::iterator isAvailable(Client& client);
};

void parseMessage(Client &client, const std::string &msg);
#endif // SERVER_HPP
