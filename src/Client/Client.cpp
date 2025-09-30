/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:34:55 by arthur            #+#    #+#             */
/*   Updated: 2025/09/30 19:03:19 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Client/Client.hpp"

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
