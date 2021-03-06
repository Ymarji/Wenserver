#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <exception>
namespace webServer
{
    class Socketexeption : public std::exception{
        private:
            std::string _msg;
        public:
            Socketexeption(std::string msg):_msg(msg){};
            const char * what () const throw (){
                return _msg.c_str();
            }
            virtual ~Socketexeption() _NOEXCEPT{}
    };
    class __socket
    {
        public:
        private:
            int _sockFD;
            struct sockaddr_in &_address;
            size_t     _addrlen;
        public:
            // __socket(int port, std::string host, struct sockaddr_in &address);
            __socket(struct sockaddr_in &address);
            int     init(int port, std::string host);
            void bindSock(int port, std::string interface);
            int const   &getsocket() const;
            ~__socket();
    };
};

#endif /* SOCKET_HPP */
