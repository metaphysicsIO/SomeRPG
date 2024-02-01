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

#include "client.h"

#define PEERPORT 8888

void Client::SetUsername()
{
    /*
     * Ask for username
     */
    std::string tmp;

    std::cout << "USERNAME: ";
    std::cin >> tmp;
    
    m_username = tmp;
}

void Client::SetPassword()
{
    /*
     * Ask for password
     */
    std::string tmp;

    std::cout << "PASSWORD: ";
    // TODO: Hide password.
    std::cin >> tmp;

    // TODO: hash password.
    m_password = tmp;
}

void Client::Login()
{
    /*
     * Login for the user.
     */

    std::cout << "[Login Screen]" << std::endl;

    SetUsername();
    SetPassword();

    // Clear the buffer.
    std::cin.clear();
    std::cin.ignore();

    std::cout << "Authenticating with server..." << std::endl;

    // Clear out the prev. buffer
    res = read(s0, buffer, 1024);

    // Add the login to the buffer.
    std::string tmp = "LOGIN:" + m_username + ":" + m_password;
    strcpy(buffer, tmp.c_str());
    write(s0, buffer, 1024);

    //TODO: Send full hash to server.

}

Client::Client()
{
    /*
     * Init for the client.
     */
    CONTINUE = true;

    // Create the socket.
    s0 = socket(AF_INET, SOCK_STREAM, 0);

    if(s0 < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    server.sin_family = AF_INET;

    server.sin_port = htons(PEERPORT);
    
    // Write resolved IP address of a server to the address structure
    memmove(&(server.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to the remote server
    res = connect(s0, (struct sockaddr*) &server, sizeof(server));
    if(res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Take initial server response
    res = read(s0, buffer, 1024);
    if(res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
    buffer[res] = 0;

    std::cout << buffer << std::endl;
}


void Client::MainLoop()
{
    while(CONTINUE)
    {
        /*
         * This is where the magic happens.
         */

        // TODO: This is where the game logic goes.

        // TODO: Create STATES {start_menu, login/register, play}

        std::string s;
        // GET USER INPUT
        printf("> ");
        std::getline(std::cin, s);

        // convert string to char
        strcpy(user_input, s.c_str());

        // SEND USER INPUT TO SERVER
        write(s0, user_input, sizeof(s));

        Commands(s);

        // READ SERVER
        res = read(s0, buffer, 1024);
        buffer[res] = 0;
        std::cout << "<SERVER>: " << buffer << std::endl;

        std::cout << std::endl;
    }

    // Close the socket
    close(s0);
}

int main()
{
    Client c;
    c.MainLoop();

}
