#include <iostream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <bitset>

#define PEERPORT 8888

class Client {
    public:
        Client();
        void MainLoop();
        void SetUsername();
        void SetPassword();
        void Login();
        void Commands(std::string);
    private:
        std::string m_username;
        std::string m_password;
        bool CONTINUE;
        int s0;
        const char* peerHost = "localhost";
        struct hostent *host = gethostbyname(peerHost);
        char buffer[1024];
        int res;
        struct sockaddr_in server;
        int serverAddrLen;
        char user_input[];
};

