#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include "socket.hpp"
#include "data.hpp"

namespace webServer{
    class server{
        private:
            __socket    sock;
            struct sockaddr_in _address;
            size_t     _addrlen;
        public:
            server(t_data data);
            void    init();
            char    *req_string();
            ~server();
    };
}
#endif /* WEBSERV_HPP */
