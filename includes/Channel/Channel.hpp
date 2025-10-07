/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:46:54 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/07 17:28:11 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"

class Client;
class Server;

enum broadcast
{
	BRCST_PRVMSG,
	BRCST_LEAVE,
	BRCST_LEAVE_MSG,
	BRCST_JOIN,
	BRCST_OP,
	BRCST_DEOP,
	BRCST_KICK,
	BRCST_TOPIC,
	BRCST_MODE,
};

class Channel {
private:
	const std::string		_name;
	std::string				_topic;
	std::string				_password;
	bool					_needInvite;
	bool					_needPassword;
	bool					_needOpTopic;
	int						_maxClient;
	std::vector<Client*>	_joinedList;
	std::vector<Client*>	_opList;
	std::vector<Client*>	_inviteList;
public:
	Channel(std::string name);
	~Channel(void);

	bool getNeedInvite(void) const;
	bool getNeedPassword(void) const;
	const std::string getName(void) const;
	const std::string getTopic(void) const;
	const std::string getPassword(void) const;
	const std::vector<Client*>& getJoinList(void) const;
	const std::vector<Client*>&     getOpList(void) const;
	const std::vector<Client*>&	getInviteList(void) const;

	void Broadcast(Client& sender, std::string msg, broadcast type, Server& server);

	void setPassword(std::string pass);
	void Op(Client& client, Server& server, Client& sender);
	void DeOp(Client& client, Server& server, Client& sender);
	void Join(Client& client, Server& server);
	void Kick(std::string nick, Server& server, std::string reason, bool leave, Client& sender);
	void Invite(Client& client, Client& sender);
	void DeInvite(Client& client, Server& server);
	void Topic(std::string topic, Server& server, Client& sender);
	void Mode(Client& sender , std::string option, Server& server);

	int findClientOp(Client& client);
	int findClientOp(std::string nick);
	int findClientJoin(Client& client);
	int findClientJoin(std::string nick);
	int findClientInvite(Client& client);
};

std::ostream& operator<<(std::ostream& o, Channel& c);


#endif
