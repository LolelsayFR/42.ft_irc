/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:34:55 by arthur            #+#    #+#             */
/*   Updated: 2025/10/01 13:41:21 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Client/Client.hpp"
#include "Server/Server.hpp"
#include "Errors/Exception.hpp"

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

int Client::getFd(void) const {
	return (this->_uid);
}

void Client::appendBuffer(const char* data, int len) {
	_buffer.append(data, len);
}

bool Client::hasMessage() const {
	return _buffer.find("\r\n") != std::string::npos;
}

std::string Client::popMessage() {
	size_t pos = _buffer.find("\r\n");
	std::string msg = _buffer.substr(0, pos);
	_buffer.erase(0, pos + 2);
	return msg;
}

bool Client::isRegistered() const
{
	return _isRegistered;
}

void Client::checkRegistration() {
	if (!_nickname.empty() && !_username.empty())
		_isRegistered = true;
}

Client* findClientByFd(std::vector<Client*> &clients, int fd) {
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getFd() == fd)
			return *it;
	}
	return NULL;
}

