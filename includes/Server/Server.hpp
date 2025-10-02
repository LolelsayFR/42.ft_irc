/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:52:45 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/02 15:06:24 by artgirar         ###   ########.fr       */
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
public:
	Server(int port, std::string password);
	~Server(void);

	int getPort(void) const;
	std::string getPassword(void) const;
	const std::vector<Client*>& getClientList(void) const;
	const std::vector<Channel*>&	getChannelList(void) const;

	void start(void);
	void parseMessage(Client &client, const std::string &msg);

	Channel* makeChannel(std::string name);

	int findClient(Client& client);
	int findClient(std::string name);
	int findClientByNick(std::string nick);
 
	int findChannel(Channel& channel);
	int findChannel(std::string name);

	void privMsgSend(Client& client, const std::string& name);
	std::vector<Client*>::iterator isAvailable(Client& client);
	void linkClientToChannel(Client& client, std::string& name);
	void destroyOneClient(std::vector<struct pollfd> &fds, int i);
};

std::ostream& operator<<(std::ostream& o, Server& s);
Client* findClientByFd(std::vector<Client*> &clients, int fd);

#endif
