/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:34:55 by arthur            #+#    #+#             */
/*   Updated: 2025/10/06 13:53:03 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Client/Client.hpp"
#include "Server/Server.hpp"
#include "Errors/Exception.hpp"
#include "Channel/Channel.hpp"

# define WHI 			"\e[1;38;5;0;107m "
# define RES			" \e[0m"

Client::Client(int fd) : _uid(fd), _isRegistered(false), _welcomeSent(false), _realname(""), _hostname(""){
	std::cout << "New client connected with fd: " << fd << std::endl;
}

Client::~Client(void) {
	std::cout << "Client with fd/id: " << this->_uid << " disconnected." << std::endl;
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

std::string Client::getRealname(void)const{
	return (this->_realname);
}

void Client::setRealname(std::string realname){
	this->_realname = realname;
}

std::string Client::getHostname(void)const{
	return (this->_hostname);
}

void Client::setHostname(std::string hostname){
	this->_hostname = hostname;
}

int Client::getUid(void) const {
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
	return (_isRegistered);
}

void Client::checkRegistration() {
	if (!_nickname.empty() && !_username.empty() && !_hostname.empty() && !_realname.empty())
		_isRegistered = true;
}

Client* findClientByFd(std::vector<Client*> &clients, int fd) {
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getUid() == fd)
			return *it;
	}
	return NULL;
}

bool Client::getWelcomeSent(void) const {
	return (this->_welcomeSent);
}
void Client::setWelcomeSent(bool val) {
	this->_welcomeSent = val;
}

//PRIVMSG handler channel
void Client::receptMessage(Channel& channel, Client& sender, std::string& msg) {
	std::string myMsg = ":" + sender.getNickname() + " PRIVMSG " + channel.getName() + " :" + msg + "\r\n";
	send(this->getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
}

//PRIVMSG handler
void Client::receptMessage(Client& sender, std::string& msg) {
	std::string myMsg = ":" + sender.getNickname() + " PRIVMSG " + this->getNickname() + " :" + msg + "\r\n";
	send(this->getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
}

//PART (leave) handler
void Client::leaveChannel(Client& sender, std::string& msg, Channel& channel) {
	std::string myMsg = ":" + sender.getNickname() + " PART " + channel.getName() + " :" + msg + "\r\n";
	send(this->getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
}

//PART (leave) handler
void Client::leaveChannel(Client& sender, Channel& channel) {
	std::string myMsg = ":" + sender.getNickname() + " PART " + channel.getName() + "\r\n";
	send(this->getUid(), myMsg.c_str(), myMsg.length(), MSG_NOSIGNAL);
}

