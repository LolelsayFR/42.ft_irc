/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:10:41 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 10:51:30 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <iostream>

/* ************************************************************************** */
/* Main parsing exception */
/* ************************************************************************** */

class ParsingException :public std::exception {};

class PortError : public ParsingException {
	const char* what(void) const throw();
};
class PasswordError : public ParsingException {
	const char* what(void) const throw();
};

#endif // EXCEPTION_HPP