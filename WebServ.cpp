#include "WebServ.hpp"
#include <fcntl.h>
namespace webServer{

    server::server(data *__data):sock(_address), _data(__data)
    {
        _addrlen = sizeof(_address);
    }

    std::string server::readReq(int fd, size_t _size){
        int res = 0;
        char buff[_size + 1];
        std::string ret;
        res = read(fd, buff, sizeof(buff) - 1);
        buff[res] = '\0';
        if (res == 0){
            std::cout << "---------------- Close Connetion" << "----------------"  << std::endl;
            close(fd);
            FD_CLR(fd, &_read_fds);
        }
        else if (res < 0){
            throw Socketexeption("cant read Req");
        }
        else{
            ret = buff;
            // while ((res = read(fd, buff, sizeof(buff) - 1)) > 0)
            // {
            //     buff[res] = '\0';
            //     ret += buff;
            // }
        }
        return ret;
    }
    void    server::init(){
        struct kevent listning_event;
        struct kevent ch_event;
        int     kq;
        int     kv; 
        int     new_socket;
        std::string req;

        // create kqueue
        kq = kqueue();
        for (data *data = _data; data != nullptr; data = data->next)
        {
            std::cout << "server  " << data->server.port << std::endl;
            master_socket = sock.init(data->server.port, data->server.host);
            
            fcntl(master_socket, F_SETFL, O_NONBLOCK);
            if (listen(master_socket, 3) < 0){
                throw Socketexeption("listen");
            }
            // add file descriptor to queue
            EV_SET(&listning_event, master_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
        }
        
        std::cout << "server listening" << std::endl;


        // add event to queue
        if (kevent(kq, &listning_event, 1, NULL, 0, 0) < 0){
            throw Socketexeption("add to queue");
        }
        while (true)
        {
            std::cout << "---------------- waiting for EVENT ----------------" << std::endl;
            kv = kevent(kq, NULL, 0, &ch_event, 1, NULL);
            if ( kv == -1)
                throw Socketexeption("kevent");

            else{
                int event_fd = ch_event.ident;
                if (ch_event.flags & EV_EOF){
                    std::cout << "---------------- Close Connetion" << "----------------"  << std::endl;
                    close(event_fd);
                }
                else if (event_fd == master_socket){
                         new_socket = accept(master_socket, (struct sockaddr *) &_address, (socklen_t *) &_addrlen);
                        if (new_socket < 0){
                            throw Socketexeption("accept");
                        }
                        std::cout << "---------------- new connecticon " <<  new_socket << "----------------"  << std::endl;
                        EV_SET(&listning_event, new_socket, EVFILT_READ, EV_ADD, 0, 0, 0);
                        if (kevent(kq, &listning_event, 1, NULL, 0, 0) < 0){
                            throw Socketexeption("add to queue");
                        }
                }
                else if (ch_event.filter & EVFILT_READ){
                        req = readReq(event_fd, ch_event.data);
                        std::cout << req << std::endl;
                        char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
                        // send(event_fd, hello, strlen(hello), 0);
                        // std::cout << " mesage sent" << std::endl;
                }
            }
        }
        
        // int client_nb = master_socket;
        // int new_socket;
        // fd_set  working_set = _read_fds;
        // char buff[1024];
        // std::string req;
        // fcntl(master_socket, F_SETFL, O_NONBLOCK);
        // if (listen(master_socket, 3) < 0){
        //     throw Socketexeption("listen");
        // }
        // std::cout << "server listening" << std::endl;
        // FD_ZERO(&_read_fds);
        // FD_SET(master_socket, &_read_fds);

        // while (1)
        // {
        //     working_set = _read_fds;
        //     std::cout << "---------------- waiting for select ----------------" << std::endl;
        //     if (select(client_nb + 1, &working_set, NULL, NULL, NULL) <= 0){
        //         throw Socketexeption("select");
        //     }
        //     for (int i = 0; i < client_nb + 1;i++){
        //         if (FD_ISSET(i, &working_set)){
        //             if (i == master_socket){
        //                  new_socket = accept(master_socket, (struct sockaddr *) &_address, (socklen_t *) &_addrlen);
        //                 if (new_socket < 0){
        //                     throw Socketexeption("accept");
        //                 }
        //                 std::cout << "---------------- new connecticon " <<  new_socket << "----------------"  << std::endl;
        //                 FD_SET(new_socket, &_read_fds);
        //                 if (client_nb < new_socket)
        //                     client_nb = new_socket;
        //             }
        //             else{
        //                 req = readReq(i);
        //                 std::cout << req << std::endl;
        //                 char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
        //                 send(i, hello, strlen(hello), 0);
        //                 std::cout << " mesage sent" << std::endl;
        //             }
        //         }
        //     }
        // }
    }
    server::~server(){}
};