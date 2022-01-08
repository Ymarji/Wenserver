#include "WebServ.hpp"
#include <fcntl.h>
namespace webServer{
    server::server(t_data data):sock(data.port, data.host, _address){
        _addrlen = sizeof(_address);
    }

    void    server::init(){
        int master_sock = sock.getsocket();
        int new_socket;
        char buff[1024];
        std::string req;
        if (listen(master_sock, 3) < 0){
            throw Socketexeption("listen");
        }
        std::cout << " server listening " << std::endl;
        std::cout << " waiting for connecction  " << std::endl;
        if ((new_socket = accept(master_sock, reinterpret_cast<struct sockaddr *>(&_address), reinterpret_cast<socklen_t *>(&_addrlen))) < 0){
            throw Socketexeption("accept connection");
        }
        int res;
        // fcntl(new_socket, F_SETFL, O_NONBLOCK);
        while ((res = read(new_socket, buff, 100)) > 0){
            req += buff;
        }
        std::cout << req; 
    }
    server::~server(){}
};