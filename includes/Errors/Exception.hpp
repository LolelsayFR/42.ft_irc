/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:10:41 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/07 22:39:32 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

#include "Irc.hpp"

/* ************************************************************************** */
/* Main parsing exception */
/* ************************************************************************** */

enum Exceptions
{
	ERR_ALREADYREGISTRED = 462,
	ERR_NOSUCHNICK = 401,
	ERR_NOTONCHANNEL = 442,
	ERR_CHANNELISFULL = 471,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_UNKNOWNMODE = 472,
	ERR_NEEDMOREPARAMS = 461,
};

class ParsingException : public std::exception
{
};

class PortErrorException : public ParsingException {
	const char* what(void) const throw();
};

class PasswordErrorException : public ParsingException {
	const char* what(void) const throw();
};

class ArgsNumberErrorException : public ParsingException {
	const char* what(void) const throw();
};

class RFCException : public std::exception
{
public:
    virtual ~RFCException() throw() {} // destructeur explicitement throw()
};

class AlreadyRegisteredException : public RFCException
{
private:
    std::string message;
	std::string fullMessage;

public:
    AlreadyRegisteredException(const std::string& arg);
    virtual ~AlreadyRegisteredException() throw(); // destructeur explicitement throw()
    virtual const char* what() const throw();
};

class NosuchNickException : public RFCException
{
	private:
	    std::string message;
		std::string fullMessage;

	public:
	    NosuchNickException(const std::string& arg);
	    virtual ~NosuchNickException() throw();// destructeur explicitement throw()
	    virtual const char* what() const throw();
};


class NotOnChannelException : public RFCException
{
	private:
		std::string message;
		std::string fullMessage;

	public:
		NotOnChannelException(const std::string& arg);
		virtual ~NotOnChannelException() throw();
		virtual const char* what() const throw();
};

class ChannelIsFullException : public RFCException
{
	private:
		std::string message;
		std::string fullMessage;

	public:
		ChannelIsFullException(const std::string& arg);
		virtual ~ChannelIsFullException() throw();
		virtual const char* what() const throw();
};

class InviteOnlyChanException : public RFCException
{
	private:
		std::string message;
		std::string fullMessage;

	public:
		InviteOnlyChanException(const std::string& arg);
		virtual ~InviteOnlyChanException() throw();
		virtual const char* what() const throw();
};

class BadChannelKeyException : public RFCException
{
	private:
		std::string message;
		std::string fullMessage;

	public:
		BadChannelKeyException(const std::string& arg);
		virtual ~BadChannelKeyException() throw();
		virtual const char* what() const throw();
};

class ChanOpPrivsNeededException : public RFCException
{
	private:
		std::string message;
		std::string fullMessage;

	public:
		ChanOpPrivsNeededException(const std::string& arg);
		virtual ~ChanOpPrivsNeededException() throw();
		virtual const char* what() const throw();
};

class UnknownModeException : public RFCException
{
	private:
		std::string message;
		std::string fullMessage;

	public:
		UnknownModeException(const std::string& arg);
		virtual ~UnknownModeException() throw();
		virtual const char* what() const throw();
};

class NeedMoreParamsException : public RFCException
{
	private:
		std::string message;
		std::string fullMessage;

	public:
		NeedMoreParamsException(const std::string& arg);
		virtual ~NeedMoreParamsException() throw();
		virtual const char* what() const throw();
};

class ClientPasswordException : public std::exception
{
	public:
		const char* what(void) const throw();
};

class SocketErrorException : public std::exception
{
	public:
		const char* what(void) const throw();
};

class SetOptionSocketErrorException : public std::exception
{
	public:
		const char* what(void) const throw();
};

class ListeningErrorException : public std::exception
{
	public:
		const char* what(void) const throw();
};

void throwRFCException(enum Exceptions exception, std::string arg);

#endif // EXCEPTION_HPP
