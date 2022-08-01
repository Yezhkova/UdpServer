// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "udpserver.h"
#include <iostream>
#define PORT	 15001

// Driver code
int main() {
    try {
        UdpServer server;
        // Creating socket file descriptor
        server.m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        memset(&server.m_servaddr, 0, sizeof(server.m_servaddr));

        // Filling server information
        server.m_servaddr.sin_family = AF_INET; // IPv4
        server.m_servaddr.sin_addr.s_addr = INADDR_ANY;
        server.m_servaddr.sin_port = htons(PORT);

        // Bind the socket with the server address
        bind(server.m_sockfd, (const struct sockaddr *)&server.m_servaddr,
             sizeof(server.m_servaddr));

        while(true){
            server.listen();
        }

        return 0;


    } catch (std::exception & e) {
        std::cout<<e.what();
    }
}
