/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:46:54 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/03 12:19:05 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"

class Client;

enum broadcast
{
	BRCST_PRVMSG,
	BRCST_LEAVE,
	BRCST_LEAVE_MSG,
	BRCST_KICK,
	BRCST_JOIN
};

class Channel {
private:
	const std::string		_name;
	std::string				_topic;
	std::string				_password;
	bool					_needInvite;
	bool					_needPassword;
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

	void Broadcast(Client& sender, std::string msg, broadcast type);
 
	void setPassword(std::string pass);
	void Op(Client& client);
	void DeOp(Client& client);
	void Join(Client& client);
	void Kick(Client& client);
	void Invite(Client& client);
	void DeInvite(Client& client);
	void Topic(std::string topic);
	void Mode(std::string option);

	int findClientOp(Client& client);
	int findClientJoin(Client& client);
	int findClientInvite(Client& client);
};

std::ostream& operator<<(std::ostream& o, Channel& c);


#endif
