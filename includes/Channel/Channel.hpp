/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:46:54 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 19:44:57 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"

class Client;

class Channel {
private:
	const std::string			_name;
	std::string					_topic;
	std::string					_password; //IF EMPTY == NONE
	bool						_needInvite;
	std::vector<Client*>	_joinedList;
	std::vector<Client*>	_opList;
	std::vector<Client*>		_inviteList;
	Channel(void);
	Channel(const Channel& other);
	Channel& operator=(const Channel& other);
public:
	//All constructor and destructor
	Channel(std::string name);
	~Channel(void);
	//All channel member functions
	void Join(Client& client);
	void Leave(Client& client);
	void Kick(Client& client);
	void Invite(Client& client);
	void Op(Client& client);
	void DeOp(Client& client);
	void Topic(std::string topic);
	void Mode(std::string option);
	//All utils
	std::vector<Client*>::iterator Channel::findClientJoin(Client& client);
	std::vector<Client*>::iterator findClientOp(Client& client);
	std::vector<Client*>::iterator findClientInvite(Client& client);
};




#endif // CHANNEL_HPP