#include "WebServ.hpp"
#include <fcntl.h>
namespace webServer{
    void    addToList(t_fdlist **root, int fd){
        t_fdlist *driver = *root;
        t_fdlist *node = new t_fdlist();
        node->fd = fd;
        node->next = nullptr;
        if (*root == nullptr){
            *root = node;
            return ;
        }
        else{
            while (driver->next){
                driver = driver->next;
            }
            driver->next = node;
        }
    }

    size_t    listSize(t_fdlist *root){
        size_t cp = 0;
        t_fdlist *driver = root;
        while(driver){
            cp++;
            driver = driver->next;
        }
        return cp;
    }

    size_t  maxfd(t_fdlist *root, size_t _max){
        size_t max = _max;
        t_fdlist *driver = root;
        while (driver)
        {
            if (driver->fd > max)
                max = driver->fd;
            driver = driver->next;
        }
        return max;
    }
    void deleteNode(t_fdlist*& head, int val)
    {
        if (head == NULL) {
            std::cout << "Element not present in the list\n";
            return;
        }
        if (head->fd == val) {
            t_fdlist* t = head;
            head = head->next;
            delete (t);
            return;
        }
        deleteNode(head->next, val);
    }



    server::server(data *_data):sock(_data->server.port, _data->server.host, _address), list_fd(nullptr){
        _addrlen = sizeof(_address);
    }

    std::string server::readReq(int fd){
        int res = 0;
        char buff[10];
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
            while ((res = read(fd, buff, sizeof(buff) - 1)) > 0)
            {
                buff[res] = '\0';
                ret += buff;
            }
        }
        return ret;
    }
    void    server::init(){
        master_socket = sock.getsocket();
        int client_nb = master_socket;
        int new_socket;
        fd_set  working_set = _read_fds;
        char buff[1024];
        std::string req;
        fcntl(master_socket, F_SETFL, O_NONBLOCK);
        if (listen(master_socket, 3) < 0){
            throw Socketexeption("listen");
        }
        std::cout << "server listening" << std::endl;
        FD_ZERO(&_read_fds);
        FD_SET(master_socket, &_read_fds);

        while (1)
        {
            working_set = _read_fds;
            std::cout << "---------------- waiting for select ----------------" << std::endl;
            if (select(client_nb + 1, &working_set, NULL, NULL, NULL) <= 0){
                throw Socketexeption("select");
            }
            for (int i = 0; i < client_nb + 1;i++){
                if (FD_ISSET(i, &working_set)){
                    if (i == master_socket){
                         new_socket = accept(master_socket, (struct sockaddr *) &_address, (socklen_t *) &_addrlen);
                        if (new_socket < 0){
                            throw Socketexeption("accept");
                        }
                        std::cout << "---------------- new connecticon " <<  new_socket << "----------------"  << std::endl;
                        FD_SET(new_socket, &_read_fds);
                        if (client_nb < new_socket)
                            client_nb = new_socket;
                    }
                    else{
                        req = readReq(i);
                        std::cout << req << std::endl;
                        char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
                        send(i, hello, strlen(hello), 0);
                        std::cout << " mesage sent" << std::endl;
                    }
                }
            }
        }
    }
    server::~server(){}
};