/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:10:41 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/01 18:14:12 by artgirar         ###   ########.fr       */
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
};

class AlreadyRegisteredException : public RFCException
{
	const char* what(void) const throw();
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

std::exception throwRFCException(enum Exceptions exception);

#endif // EXCEPTION_HPP
