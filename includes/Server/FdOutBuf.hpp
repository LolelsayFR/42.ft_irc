/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdOutBuf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artgirar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:33:44 by artgirar          #+#    #+#             */
/*   Updated: 2025/09/30 16:08:52 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FdOutBuf_hpp
# define FdOutBuf_hpp
# include "Irc.hpp"

class FdOutBuf : public std::streambuf {
private:
	int fd_;

protected:
	virtual int_type overflow(int_type c);
	virtual std::streamsize xsputn(const char* s, std::streamsize num);

public:
	FdOutBuf(int fd);
};

#endif
