/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lang.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:08:58 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/05 16:08:56 by arthur           ###   ########.fr       */
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
# define E_ALREADY_REGISTERED ":You may not reregister."
# define E_ERR_WRONGPASSWORD  "🛑 Error : Wrong password send to connect to server."
# define E_SOCKETERROR        "🛑 Error : Socket creation error."
# define E_SETSOCKETOPTERROR  "🛑 Error : Socket setting option error."
# define E_LISTENING_ERROR    "🛑 Error : Server socket listening error."
# define E_NOSUCHNICK        "🛑 Error : No such nick/channel."

#endif // LANG_HPP
