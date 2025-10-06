/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lang.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:08:58 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/06 14:33:21 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LANG_HPP
# define LANG_HPP

/* ************************************************************************** */
/* Exception */
/* ************************************************************************** */

# define E_PARSING_PORT_ERROR "🛑 Error : The port number provided for the argument is invalid."
# define E_PARSING_PASS_ERROR "🛑 Error : Bad password sent as an argument."
# define E_PARSING_ARGS_ERROR "🛑 Error : Invalid argument number."
# define E_ERR_WRONGPASSWORD  "🛑 Error : Wrong password send to connect to server."
# define E_SOCKETERROR        "🛑 Error : Socket creation error."
# define E_SETSOCKETOPTERROR  "🛑 Error : Socket setting option error."
# define E_LISTENING_ERROR    "🛑 Error : Server socket listening error."
# define E_ALREADY_REGISTERED ": You may not reregister."
# define E_NOSUCHNICK        ": No such nick/channel."
# define E_NOTONCHANNEL     ": You're not on that channel."

#endif // LANG_HPP
