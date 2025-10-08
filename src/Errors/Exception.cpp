/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:35:21 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/08 15:07:48 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Errors/Exception.hpp"


/* ************************************************************************** */
/* Main parsing exception */
/* ************************************************************************** */

//Password arg error exception (Parsing)
const char* PasswordErrorException::what(void) const throw() {
	return (E_PARSING_PASS_ERROR);
}

//Port arg error exception (Parsing)
const char* PortErrorException::what(void) const throw() {
	return (E_PARSING_PORT_ERROR);
}

//Arg number error exception (Parsing)
const char* ArgsNumberErrorException::what(void) const throw() {
	return (E_PARSING_ARGS_ERROR);
}

//RFC Exceptions
AlreadyRegisteredException::AlreadyRegisteredException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 462 " + clientNick + " " + this->message + E_ALREADY_REGISTERED;
}

AlreadyRegisteredException::~AlreadyRegisteredException() throw() {}



NosuchNickException::NosuchNickException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 401 " + clientNick + " " + this->message + E_NOSUCHNICK;
}

NosuchNickException::~NosuchNickException() throw() {}



NotOnChannelException::NotOnChannelException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 442 " + clientNick + " " + this->message + E_NOTONCHANNEL;
}

NotOnChannelException::~NotOnChannelException() throw() {}


ChannelIsFullException::ChannelIsFullException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 471 " + clientNick + " " + this->message + E_CHANNELISFULL;
}

ChannelIsFullException::~ChannelIsFullException() throw() {}


InviteOnlyChanException::InviteOnlyChanException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 473 " + clientNick + " " + this->message + E_INVITEONLYCHAN;
}

InviteOnlyChanException::~InviteOnlyChanException() throw() {}


BadChannelKeyException::BadChannelKeyException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 475 " + clientNick + " " + this->message + E_BADCHANNELKEY;
}

BadChannelKeyException::~BadChannelKeyException() throw() {}

ChanOpPrivsNeededException::ChanOpPrivsNeededException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 482 " + clientNick + " " + this->message + E_CHANOPRIVSNEEDED;
}

ChanOpPrivsNeededException::~ChanOpPrivsNeededException() throw() {}

UnknownModeException::UnknownModeException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 472 " + clientNick + " " + this->message + E_UNKNOWNMODE;
}

UnknownModeException::~UnknownModeException() throw() {}

NeedMoreParamsException::NeedMoreParamsException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 461 " + clientNick + " " + this->message + E_NEEDMOREPARAMS;
}

NeedMoreParamsException::~NeedMoreParamsException() throw() {}

NosuchChannelException::NosuchChannelException(const std::string& arg, std::string clientNick) : message(arg)
{
	this->fullMessage = " 403 " + clientNick + " " + this->message + E_NOSUCHCHANNEL;
}

NosuchChannelException::~NosuchChannelException() throw() {}


const char* AlreadyRegisteredException::what() const throw() {
	return (this->fullMessage.c_str());
}

const char* NosuchNickException::what() const throw() {
	return (this->fullMessage.c_str());
}

const char* NotOnChannelException::what(void) const throw() {
	return (this->fullMessage.c_str());
}

const char* ChannelIsFullException::what(void) const throw() {
	return (this->fullMessage.c_str());
}

const char* InviteOnlyChanException::what(void) const throw() {
	return (this->fullMessage.c_str());
}

const char* BadChannelKeyException::what(void) const throw() {
	return (this->fullMessage.c_str());
}

const char* ChanOpPrivsNeededException::what(void) const throw() {
	return (this->fullMessage.c_str());
}

const char* UnknownModeException::what(void) const throw() {
	return (this->fullMessage.c_str());
}

const char* NeedMoreParamsException::what(void) const throw() {
	return (this->fullMessage.c_str());
}

const char* NosuchChannelException::what(void) const throw() {
	return (this->fullMessage.c_str());
}

void throwRFCException(enum Exceptions exception,std::string arg, std::string clientNick) {
	switch (exception)
	{
		case ERR_ALREADYREGISTRED:
			throw AlreadyRegisteredException(arg, clientNick);
		case ERR_NOSUCHNICK:
			throw NosuchNickException(arg, clientNick);
		case ERR_NOTONCHANNEL:
			throw NotOnChannelException(arg, clientNick);
		case ERR_CHANNELISFULL:
			throw ChannelIsFullException(arg, clientNick);
		case ERR_INVITEONLYCHAN:
			throw InviteOnlyChanException(arg, clientNick);
		case ERR_BADCHANNELKEY:
			throw BadChannelKeyException(arg, clientNick);
		case ERR_CHANOPRIVSNEEDED:
			throw ChanOpPrivsNeededException(arg, clientNick);
		case ERR_UNKNOWNMODE:
			throw UnknownModeException(arg, clientNick);
		case ERR_NEEDMOREPARAMS:
			throw NeedMoreParamsException(arg, clientNick);
		case ERR_NOSUCHCHANNEL:
			throw NosuchChannelException(arg, clientNick);
		default :
			throw RFCException();
	}
}

const char* ClientPasswordException::what(void) const throw () {
	return (E_ERR_WRONGPASSWORD);
}

const char* SocketErrorException::what(void) const throw () {
	return (E_SOCKETERROR);
}

const char *SetOptionSocketErrorException::what(void) const throw() {
	return(E_SETSOCKETOPTERROR);
}

const char* ListeningErrorException::what(void) const throw () {
	return (E_LISTENING_ERROR);
}
