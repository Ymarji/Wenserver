// #include <stdio.h>
// #include <sys/socket.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <arpa/inet.h>
// #define PORT 8888

// int main(int argc, char const *argv[])
// {
//     int sock = 0; long valread;
//     struct sockaddr_in serv_addr;
//     char hello[1024] ={0};
//     char buffer[1024] = {0};
//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         printf("\n Socket creation error \n");
//         return -1;
//     }

//     memset(&serv_addr, '0', sizeof(serv_addr));

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);
// 	serv_addr.sin_addr.s_addr = INADDR_ANY;

//     // Convert IPv4 and IPv6 addresses from text to binary form
//     // if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
//     // {
//     //     printf("\nInvalid address/ Address not supported \n");
//     //     return -1;
//     // }

//         if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//         {
//             perror("cone ");
//             printf("\nConnection Failed \n");
//             return -1;
//         }
//     while (true)
//     {
//         valread = read( 0 , hello, 1024);
//         hello[valread] = '\0';
//         send(sock , hello , strlen(hello) , 0 );
//         printf("Hello message sent %s\n", hello);
//         printf("%s\n",buffer );
//     }

//     return 0;
// }

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 5555
#define MAXMSG 1024

int read_from_client(int filedes)
{
    char buffer[MAXMSG];
    int nbytes;

    nbytes = read(filedes, buffer, MAXMSG);
    if (nbytes < 0)
    {
        /* Read error. */
        perror("read");
        exit(EXIT_FAILURE);
    }
    else if (nbytes == 0)
        /* End-of-file. */
        return -1;
    else
    {
        /* Data read. */
        fprintf(stderr, "Server: got message: `%s'\n", buffer);
        return 0;
    }
}

int make_socket(uint16_t port)
{
    int sock;
    struct sockaddr_in name;

    /* Create the socket. */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Give the socket a name. */
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    return sock;
}

int main(void)
{
    extern int make_socket(uint16_t port);
    int sock;
    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    size_t size;

    /* Create the socket and set it up to accept connections. */
    sock = make_socket(PORT);
    if (listen(sock, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
	printf("Listener on port %d \n", PORT);
    /* Initialize the set of active sockets. */
    FD_ZERO(&active_fd_set);
    FD_SET(sock, &active_fd_set);

    while (1)
    {
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        /* Service all the sockets with input pending. */
        for (i = 0; i < FD_SETSIZE; ++i)
            if (FD_ISSET(i, &read_fd_set))
            {
                if (i == sock)
                {
                    /* Connection request on original socket. */
                    int new1;
                    size = sizeof(clientname);
                    new1 = accept(sock,
                                  (struct sockaddr *)&clientname,
                                  (socklen_t *)&size);
                        puts("new connections ...");
                    if (new1 < 0)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    fprintf(stderr,
                            "Server: connect from host %s, port %hd.\n",
                            inet_ntoa(clientname.sin_addr),
                            ntohs(clientname.sin_port));
                    FD_SET(new1, &active_fd_set);
                }
                else
                {
                    /* Data arriving on an already-connected socket. */
                        puts("============ req ==============");
                    if (read_from_client(i) < 0)
                    {
                        close(i);
                        FD_CLR(i, &active_fd_set);
                    }
                        puts("==============================");
                }
            }
    }
}