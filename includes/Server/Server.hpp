/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:52:45 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/02 15:29:47 by emaillet         ###   ########.fr       */
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
	const std::vector<Channel*>&	getChannelList(void) const;
	const std::vector<Client*>&	getClientList(void) const;
	int getPort(void) const;
	std::string getPassword(void) const;
	void start(void);
	Channel* makeChannel(std::string name);
	void linkClientToChannel(Client& client, std::string& name);
	void privMsgSend(Client& client, const std::string& name);
	int findClient(Client& client);
	int findClient(std::string name);
	int findClientByNick(std::string nick);
	int findChannel(Channel& channel);
	int findChannel(std::string name);
	void parseMessage(Client &client, const std::string &msg);
	std::vector<Client*>::iterator isAvailable(Client& client);
	void destroyOneClient(std::vector<struct pollfd> &fds, int i);

	void clientLeaveChannel(Client& client, const std::string& name);
};

std::ostream& operator<<(std::ostream& o, Server& s);
Client* findClientByFd(std::vector<Client*> &clients, int fd);

#endif // SERVER_HPP
