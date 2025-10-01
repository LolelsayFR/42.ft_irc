/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:52:45 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/01 18:06:55 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"
#include "Client/Client.hpp"
#include "Channel/Channel.hpp"

class Client;

class Server {
private:
	const int				_port;
	const std::string		_password;
	std::vector<Client*>	_clientList;
	std::vector<Channel*>	_channelList;
    //sockaddr_in			_serverAddress;
public:
	//All constructor and destructor
	Server(int port, std::string password);
	~Server(void);
	//All other member functions
	void start(void);
	int findClient(Client& client);
	int findChannel(Channel& client);
	std::vector<Client*>::iterator isAvailable(Client& client);
};

void parseMessage(Client &client, const std::string &msg);
#endif // SERVER_HPP
