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
#include "server.h"

Server::Server()
{
        // Init all client_socket[] to 0 to not check.
        for(int i = 0; i < max_clients; ++i)
        {
            client_socket[i] = 0;
        }
    
        // Create the master socket
        master_socket = socket(AF_INET, SOCK_STREAM, 0);
    
        // Type of socket created
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
    
        // Bind socket to localhost:port
        bind(master_socket, (struct sockaddr *) &address, sizeof(address));
        printf("Listening on port %d\n", PORT);
    
        // Set max pending connections to 1/4th of MAX_CLIENTS.
        listen(master_socket, (MAX_CLIENTS / 4));
    
        // Accept incoming connection
        addr_size = sizeof(address);
        std::cout << "Waiting for connections." << std::endl;
}

void Server::MainLoop()
{
    while(true)
    {
        // clear the socket set
        FD_ZERO(&readfds);

        // Add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        // Add child sockets to the set.
        for(int i = 0; i < MAX_CLIENTS; ++i)
        {
            // socket descriptor
            sd = client_socket[i];

            // if it is a valid socket descriptor, add to read list
            if(sd > 0)
            {
                FD_SET(sd, &readfds);
            }

            // If this socket descriptor is the highest one, elevate it to the
            // top.
            if(sd > max_sd)
            {
                max_sd = sd;
            }
        }

        // Wait for an activity from the socket. We are setting the timeout to
        // NULL so that it doesn't randomly break.
        activity = select((max_sd + 1), &readfds, NULL, NULL, NULL);

        if(activity < 0)
            std::cout << "SELECT ERROR" << std::endl;

        if(FD_ISSET(master_socket, &readfds))
        {
            // Master socket interactions.
            new_socket = accept(master_socket, (struct sockaddr *) &address, (socklen_t*) &addr_size);

            // Print out new connection and socket number.
            printf("[New Connection] Socket fd is %d, IP is [%s], PORT is [%d]\n", new_socket, inet_ntoa(address.sin_addr), (address.sin_port));

            // Inform the new connection that it is connected.
            send(new_socket, message, strlen(message), 0);

            std::cout << "[New Connection] MOTD send to new client!" << std::endl;

            // Add the new socket to the array of sockets.
            for(int i = 0; i < MAX_CLIENTS; ++i)
            {
                if(client_socket[i] == 0)
                {
                    // Make sure to add the socket to an empty one.
                    client_socket[i] = new_socket;
                    printf("[New Connection] Add to socket %d\n", i);
                    break;
                }
            }
        }else{
            for(int i = 0; i < MAX_CLIENTS; ++i)
            {
                // Client interactions.
                sd = client_socket[i];

                if(FD_ISSET(sd, &readfds))
                {
                    valread = read(sd, buffer, 1024);
                    
                    // Check if it was for closing connection
                    if(strcmp(buffer, "KILLSERV") == strcmp("KILLSERV", "KILLSERV"))
                    {
                        // Print out the disconnection.
                        getpeername(sd, (struct sockaddr *) &address, (socklen_t*) &addr_size);
                        printf("[%s]:[%d] disconnected.\n", inet_ntoa(address.sin_addr), (address.sin_port));

                        // Close the socket and mark it as empty.
                        close(sd);
                        client_socket[i] = 0;
                        
                        std::cout << "Shutting down server..." << std::endl;

                        // kill entire program.
                        close(master_socket);
                        exit(0);
                    }else{
                        /*
                         * This is the main loop.
                         */

                        // Login test.
                        if(strcmp(buffer, "LOGIN:test:1234") == strcmp("LOGIN:test:1234","LOGIN:test:1234"))
                        {
                            std::string tmp_str = "user [test] was logged in.";
                            strcpy(buffer, tmp_str.c_str());
                            getpeername(sd, (struct sockaddr *) &address, (socklen_t*) &addr_size);
                            printf("[%s]:[%d] LOGGED IN.\n", inet_ntoa(address.sin_addr), (address.sin_port));
                        }

                        // Check for a command
                        if(buffer[0] == '/')
                        {
                            // QUIT
                            if(buffer[1] == 'q' && buffer[2] == 'u' && buffer[3] == 'i' && buffer[4] == 't')
                            {
                                printf("[%s]:[%d] disconnected.\n", inet_ntoa(address.sin_addr), (address.sin_port));
                                close(sd);
                                client_socket[i] = 0;
                            }

/*

//
// DISABLING SAY UNTIL I CAN FIGURE THIS OUT.
//
                            // SAY TEST
                            if(buffer[1] == 's' && buffer[2] == 'a' && buffer[3] == 'y')
                            {
                                // Convert the buffer to a string.
                                std::string tmp_str(buffer);
                                std::string output = "\n";
                                // Arrange the text properly. 
                                // TODO: Fix this to make it look nicer.
                                output.insert(0, tmp_str.substr(5,-1));
                                output.insert(0, "] ");
                                output.insert(0, std::to_string(address.sin_port));
                                output.insert(0, ":");
                                output.insert(0, inet_ntoa(address.sin_addr));
                                output.insert(0,  "[");
                                
                                strcpy(buffer, output.c_str());
                                buffer[valread] = '\0';

                                // Send to all active sockets.
                                for(int j = 0; j < max_sd; ++i)
                                {
                                    // May need a fresh line per send?
                                    //strcpy(buffer, output.c_str());
                                    //buffer[valread] = '\0';
                                    sda = client_socket[j];
                                    if((sda > 0) && (sd != sda))
                                    {
                                        std::cout << sda << std::endl;
                                        send(sda, buffer, strlen(buffer), 0);
                                    }
                                }

                                // print to the server.
                                std::cout << output << std::endl;
                            }
*/
                        }

                        // Sends a message to the client anyways to let it
                        // knows it still cares.
                            
                        // Don't forget the end of the string has to terminate.
                        buffer[valread] = '\0';
                        // Echo message RECV back to given client.
                        send(sd, buffer, strlen(buffer), 0);
                    }
                }
            }
        }
    }
}
