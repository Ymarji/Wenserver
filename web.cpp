#include "WebServ.hpp"

data  *fakedata(){
    data *_data = new data;
    {
        //1st server block
        _data->server.port = 5000;
        _data->server.host = "127.0.0.1";
        _data->server.server_name = "exemple.com";
        _data->server.root = "/src";
        //location
        _data->server.location.root = "/";
        _data->server.location.autoindex = false;
        _data->server.location.index = "index.html";
        _data->server.location.mehtode = 0;
        _data->server.location.is_red = true;
        _data->server.location.upload_enable = false;
        _data->server.location.upload_store = "";
        //redirection
        _data->server.redirection.code = 301;
        _data->server.redirection._url = "/";
    }
    data *_n_data = new data;
    _data->next = _n_data;
    {
        //2nd server block
        _n_data->server.port = 3000;
        _n_data->server.host = "localhost";
        _n_data->server.server_name = "exemple.com";
        _n_data->server.root = "/src";
        //location
        _n_data->server.location.root = "/";
        _n_data->server.location.autoindex = false;
        _n_data->server.location.index = "index.html";
        _n_data->server.location.mehtode = 0;
        _n_data->server.location.is_red = true;
        _n_data->server.location.upload_enable = false;
        _n_data->server.location.upload_store = "";
        //redirection
        _n_data->server.redirection.code = 301;
        _n_data->server.redirection._url = "/";
    }
    return _data;
}
int main(){
    try
    {
        data *_data = fakedata();
        webServer::server Server(_data);
        Server.init();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error :"<< e.what()  << std::endl;
    }
    return 0;
}