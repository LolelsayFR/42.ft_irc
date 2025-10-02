/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:43:51 by arthur            #+#    #+#             */
/*   Updated: 2025/10/02 12:43:42 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Irc.hpp"
#include "Server/Server.hpp"

class Server;
class Client {
private :
	const int			_uid;
	std::string			_username;
	std::string			_nickname;
	std::string			_buffer;
	bool				_isRegistered;
	bool				_welcomeSent;
public :
	Client(int fd);
	~Client(void);

	std::string getUsername(void) const;
	std::string getNickname(void) const;
	void setNickname(std::string nickname);
	void setUsername(std::string username);
	int getFd(void) const;
	void appendBuffer(const char* data, int len);
	bool hasMessage() const;
	std::string popMessage();
	bool isRegistered(void) const;
	void checkRegistration();
	bool getWelcomeSent(void) const;
	void setWelcomeSent(bool val);
};

Client* findClientByFd(std::vector<Client*> &clients, int fd);
