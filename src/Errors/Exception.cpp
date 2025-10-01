/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:35:21 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/01 18:14:24 by artgirar         ###   ########.fr       */
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

const char* AlreadyRegisteredException::what(void) const throw() {
	return (E_ALREADY_REGISTERED);
}

std::exception throwRFCException(enum Exceptions exception) {
	switch (exception)
	{
		case ERR_ALREADYREGISTRED:
			throw AlreadyRegisteredException();
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
