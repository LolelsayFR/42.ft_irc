/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:34:55 by arthur            #+#    #+#             */
/*   Updated: 2025/10/01 11:04:28 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Client/Client.hpp"
#include "Server/Server.hpp"

Client::Client(int fd) : _uid(fd){
	std::cout << "New client connected with fd: " << fd << std::endl;
}

Client::~Client(void) {
	std::cout << "Client with fd: " << this->_uid << " disconnected." << std::endl;
}

std::string Client::getUsername(void)const{
	return (this->_username);
}

void Client::setUsername(std::string username){
	this->_username = username;
}

std::string Client::getNickname(void)const{
	return (this->_nickname);
}

void Client::setNickname(std::string nickname){
	this->_nickname = nickname;
}

void Client::extractUsername(const std::string& buffer) {
	std::string prefix;
	size_t start;

	if (this->_username.empty())
		prefix = "USER ";
	start = buffer.find(prefix);
		if (start != std::string::npos) {
		start += prefix.length();
		size_t end = buffer.find(' ', start);
		if (this->_username.empty() && end != std::string::npos) {
			this->setUsername(buffer.substr(start, end - start));
			return;
		} else {
			this->setUsername(buffer.substr(start));
			return;
		}
	}
}

void Client::extractNickname(const std::string& buffer) {
	std::string prefix;
	size_t start;

	if (this->_nickname.empty())
		prefix = "NICK ";
	start = buffer.find(prefix);
		if (start != std::string::npos) {
		start += prefix.length();
		size_t end = buffer.find(' ', start);
		if (this->_nickname.empty() && end != std::string::npos) {
			this->setNickname(buffer.substr(start, end - start));
			return;
		} else {
			this->setNickname(buffer.substr(start));
			return;
		}
	}
}

void Client::initClient(std::vector<Client *> &_clientList, char *buffer, int i)
{
	(void)i;
	std::cout << "Init client with buffer: " << buffer << std::endl;
	if (_clientList[_clientList.size() - 1]->getNickname().empty())
	{
		_clientList[_clientList.size() - 1]->extractNickname(buffer);
		std::cout << "Username set to: " << _clientList[_clientList.size() - 1]->getUsername() << std::endl;
		std::cout << "Nickname set to: " << _clientList[_clientList.size() - 1]->getNickname() << std::endl;
	}
	else if (_clientList[_clientList.size() - 1]->getUsername().empty())
	{
		_clientList[_clientList.size() - 1]->extractUsername(buffer);
		std::cout << "Username set to: " << _clientList[_clientList.size() - 1]->getUsername() << std::endl;
		std::cout << "Nickname set to: " << _clientList[_clientList.size() - 1]->getNickname() << std::endl;
	}
}
