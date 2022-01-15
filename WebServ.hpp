#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include "socket.hpp"
#include "data.hpp"

namespace webServer{
    typedef struct s_fdlist{
        int fd;
        struct s_fdlist    *next; 
        s_fdlist():fd(0), next(nullptr){};
    }   t_fdlist;

    void    addToList(t_fdlist **root, int fd);

    size_t    listSize(t_fdlist *root);
    class server{
        private:
        typedef int sock_fd;
            __socket    sock;
            struct sockaddr_in _address;
            sock_fd     master_socket;
            fd_set      _read_fds;
            size_t      _addrlen;
            t_fdlist    *list_fd;
        public:
            server(data *data);
            void    init();
            char    *req_string();
            std::string readReq(int fd);
            ~server();
    };
}
#endif /* WEBSERV_HPP */