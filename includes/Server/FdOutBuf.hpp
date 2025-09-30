/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdOutBuf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artgirar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:33:44 by artgirar          #+#    #+#             */
/*   Updated: 2025/09/30 15:44:20 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class FdOutBuf : public std::streambuf {
private:
    int fd_;
    
protected:
    virtual int_type overflow(int_type c) {
        if (c != EOF) {
            char z = c;
            if (write(fd_, &z, 1) != 1) {
                return EOF;
            }
        }
        return c;
    }
    
    virtual std::streamsize xsputn(const char* s, std::streamsize num) {
        return write(fd_, s, num);
    }
    
public:
    FdOutBuf(int fd) : fd_(fd) {}
};
