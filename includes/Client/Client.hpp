/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:43:51 by arthur            #+#    #+#             */
/*   Updated: 2025/10/02 16:10:32 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Irc.hpp"
#include "Server/Server.hpp"

class Server;
class Channel;

class Client {
private :
	const int		_uid;
	bool			_isRegistered;
	bool			_welcomeSent;
	std::string		_username;
	std::string		_nickname;
	std::string		_buffer;
public :
	Client(int fd);
	~Client(void);

	int getUid(void) const;
	bool hasMessage() const;
	bool isRegistered(void) const;
	bool getWelcomeSent(void) const;
	std::string getUsername(void) const;
	std::string getNickname(void) const;

	void checkRegistration();
	void setWelcomeSent(bool val);
	void setNickname(std::string nickname);
	void setUsername(std::string username);
	void appendBuffer(const char* data, int len);

	void receptMessage(Channel& channel, Client& sender, std::string& msg);
	void receptMessage(Client& sender, std::string& msg);

	void leaveChannel(std::string& msg, Channel& channel);
	void leaveChannel(Channel& channel);

	std::string popMessage();
};

Client* findClientByFd(std::vector<Client*> &clients, int fd);
