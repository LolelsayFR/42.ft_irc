/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:10:41 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/07 21:14:19 by arthur           ###   ########.fr       */
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
	ERR_ALREADYREGISTRED = 461,
	ERR_NOSUCHNICK = 401,
	ERR_NOTONCHANNEL = 442,
	ERR_CHANNELISFULL = 471
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
