/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:35:21 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 10:56:15 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lang.hpp"
#include "Exception.hpp"

/* ************************************************************************** */
/* Main exception */
/* ************************************************************************** */

//Password arg error exception (Parsing)
const char* PasswordError::what(void) const throw() {
	return (E_PARSING_PASS_ERROR);
}

//Port arg error exception (Parsing)
const char* PortError::what(void) const throw() {
	return (E_PARSING_PORT_ERROR);
}