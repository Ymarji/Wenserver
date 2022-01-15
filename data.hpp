#ifndef DATA_HPP
#define DATA_HPP
#include <string>
#include <map>

    struct s_server
    {
        int port;
        std::string host;
        int client_max_body_size;
        std::string server_name;
        std::string root;
        std::map<int, std::string> error_pages;
        struct s_location
        {
            std::string root;
            bool autoindex;
            std::string index;
            int mehtode;
            bool is_red;
            bool upload_enable;
            std::string upload_store;
        } location;
        struct s_red
        {
            int code;
            std::string _url;
        } redirection;
    };
    struct data
    {
        struct s_server server;
        struct data *next;
    };
#endif /* DATA_HPP */
