/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:43:51 by arthur            #+#    #+#             */
/*   Updated: 2025/10/02 12:55:14 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Irc.hpp"
#include "Server/Server.hpp"

class Server;
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

	std::string popMessage();
};

Client* findClientByFd(std::vector<Client*> &clients, int fd);
