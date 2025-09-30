/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:10:41 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 14:45:53 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

#include "Irc.hpp"

/* ************************************************************************** */
/* Main parsing exception */
/* ************************************************************************** */

class ParsingException :public std::exception {};

class PortErrorException : public ParsingException {
	const char* what(void) const throw();
};

class PasswordErrorException : public ParsingException {
	const char* what(void) const throw();
};

class ArgsNumberErrorException : public ParsingException {
	const char* what(void) const throw();
};

#endif // EXCEPTION_HPP
