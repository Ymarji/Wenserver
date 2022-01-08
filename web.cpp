#include "WebServ.hpp"
#include "data.hpp"

t_data  fakedata(){
    t_data data;
    data.port = 8080;
    data.host = "127.0.0.1";
    data.server_name = "example.com";
    return data;
}
int main(){
    
    try
    {
        webServer::server Server(fakedata());
        Server.init();
    }
    catch(const std::exception& e)
    {
        std::cerr << " error "<< e.what() << std::endl;
    }
    return 0;
}