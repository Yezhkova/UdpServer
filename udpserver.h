#ifndef UDPSERVER_H
#define UDPSERVER_H
#pragma once
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include "clientdata.h"
#include <map>
#include <iostream>

class UdpServer
{

public:
    int                                 m_sockfd;
    struct sockaddr_in                  m_servaddr;
    struct sockaddr_in                  m_cliaddr;

    std::map<std::string, ClientData>   m_clientmap;
public:
    UdpServer()
    {
        std::cout<<"Server starts working\n";
    }
    ~UdpServer()
    {
        ::close(m_sockfd);
    }
    void listen();

private:
    void sendOldToNewUserList();
    void sendChangesToOldClients(std::string username);
};

#endif // UDPSERVER_H
