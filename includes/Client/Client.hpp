/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:43:51 by arthur            #+#    #+#             */
/*   Updated: 2025/10/01 13:41:14 by arthur           ###   ########.fr       */
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
	//std::ostream		_clientStream;
	//std::vector<Channel&> <--- All channel joined;
public :
	Client(int fd);
	~Client(void);
	// const int getUid(void)const;
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


	// std::string getNickname(void)const;
	// void setNickname(std::string nickname);
	//Client Command
	//void join(std::string channel);
};

Client* findClientByFd(std::vector<Client*> &clients, int fd);
