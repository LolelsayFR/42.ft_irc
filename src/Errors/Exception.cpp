/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:35:21 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 15:17:45 by emaillet         ###   ########.fr       */
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
