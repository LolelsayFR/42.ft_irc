/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:52:45 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/07 19:17:27 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"
#include "Client/Client.hpp"
#include "Channel/Channel.hpp"

# define WLCM_MESSAGE 			"\e[48;2;20;100;20;1m Welcome to IRCDebout \e[0m "


class Client;

class Server {
private:
	const int				_port;
	const std::string		_password;
	std::string				_hostName;
	std::vector<Client*>	_clientList;
	std::vector<Client*>	_setupList;
	std::vector<Channel*>	_channelList;
	std::vector<struct pollfd>	_fds;
public:
	Server(int port, std::string password);
	~Server(void);

	int getPort(void) const;
	std::string getPassword(void) const;
	std::string getHost(void) const;
	std::vector<struct pollfd>& getFds(void);
	const std::vector<Client*>& getClientList(void) const;
	const std::vector<Client*>& getSetupList(void) const;
	const std::vector<Channel*>&	getChannelList(void) const;

	void start(void);
	void parseMessage(Client &client, const std::string &msg);

	Channel* makeChannel(std::string name);

	void welcomeUser(Client *client);

	int findClient(Client& client);
	int findClient(int fd);
	int findClient(std::string name);
	int findClientByNick(std::string nick);
	int findClientSetup(int fd);

	int findChannel(Channel& channel);
	int findChannel(std::string name);

	void setClientNick(Client& client, std::string& nick);
	void privMsgSend(Client& client, const std::string& name);
	void sendModeChannel(Client& client, const std::string& name);
	std::vector<Client*>::iterator isAvailable(Client& client);
	void linkClientToChannel(Client& client, std::string& name);
	void destroyOneClient(std::vector<struct pollfd> &fds, int i);
	void destroyOneWaiting(std::vector<struct pollfd> &fds, int i);

	void clientLeaveChannel(Client& client, const std::string& name);

	void clientSetupHandler(int i, int n, char *buffer);
	void clientHandler(int i, int n, char *buffer);
};

std::ostream& operator<<(std::ostream& o, Server& s);
Client* findClientByFd(std::vector<Client*> &clients, int fd);

#endif
