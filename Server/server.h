#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <poll.h>

#define MAX_CLIENTS 100
#define PORT 8888


class Server {
    public:
        Server();
        void MainLoop();
    private:
        int master_socket;
        socklen_t addr_size;
        int new_socket;
        int client_socket[MAX_CLIENTS];
        int max_clients = MAX_CLIENTS;
        int max_sd;
        int activity;
        int valread;
        int sd;    
        //int sda;
        struct sockaddr_in address;
        char buffer[1024];
        fd_set readfds;
        const char *message = ".:: MOTD ::.\r\nWelcome to this test server.\r\n";
};

