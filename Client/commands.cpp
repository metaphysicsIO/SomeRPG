#include <iostream>
#include "client.h"

void Client::Commands(std::string s)
{
    /*
     * Check if user inserted a command.
     * Returns 0 if not.
     */

    if(s == "/quit")
    {
        printf("[LOCAL] You have disconnected from the server.\n");
        exit(0);
    }
    else if(s == "commands")
    {
        printf("[LOCAL] The commands list are as follows:\n");
        printf("\tlogin : Login to the server.\n");
        printf("\t/say : Write to all connected clients.\n");
        printf("\t/quit : Disconnects from the server and closes the client.\n");
        printf("\tKILLSERV : Shuts the server down from the client.\n");
    }
    else if(s == "login")
    {
        Login();
    }
    else if(s == "/say")
    {
        ;
    }
    else if(s == "KILLSERV")
    {
        printf("[LOCAL] You have killed the server.\n");
    }
    else
    {
        printf("[LOCAL] Command not found. Try 'commands' for help.\n");
    }
}
