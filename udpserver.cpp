#include "udpserver.h"
#include <arpa/inet.h>
#include <unistd.h>
#include "serialization.h"
#include <cereal/types/string.hpp>

#define MAXLINE  (1024*1024)

void UdpServer::sendOldToNewUserList()
{
    if(m_clientmap.size() == 0) std::cout<< "map empty\n";
    for(auto& [key, val] : m_clientmap)
    {
        std::stringstream ss;
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive

        CmdClientinfo cmd {
            .username = key,
            .address = val.getAddress(),
            .port = std::to_string(val.getPort())
        };
        oarchive(cmd);



        size_t packetLength = ss.str().length();
        char * buffer = new char[packetLength + 2];
        uint16_t * packetLengthPtr = reinterpret_cast<uint16_t *>(buffer);
        * packetLengthPtr = packetLength;
        std::memcpy(buffer + 2, ss.str().c_str(), packetLength);

        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        sendto(sockfd, buffer, packetLength+2,
               MSG_CONFIRM, (const struct sockaddr *) &m_cliaddr,
               sizeof(m_cliaddr));
        ::close(sockfd);
    }
}

void UdpServer::sendChangesToOldClients(std::string username)
{
    const char * message = const_cast<char*>(username.c_str());


//    std::string message = 'u' + username + ';' + inet_ntoa(m_cliaddr.sin_addr) +';' + std::to_string(m_cliaddr.sin_port);

    std::stringstream ss;
    cereal::BinaryOutputArchive oarchive(ss);
    CmdClientinfo cmd {
        .username = username,
        .address = inet_ntoa(m_cliaddr.sin_addr),
        .port = std::to_string(m_cliaddr.sin_port)
    };
    oarchive(cmd);

    size_t packetLength = ss.str().length();
    char * buffer = new char[packetLength+2];
    std::memcpy(buffer+2, ss.str().c_str(), packetLength);


    for(auto& [key, val] : m_clientmap)
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = val.getPort();
        addr.sin_addr.s_addr = inet_addr(val.getAddress());
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        sendto(sockfd, buffer, packetLength+2,
               MSG_CONFIRM, (const struct sockaddr *) &addr,
               sizeof(addr));
        ::close(sockfd);
    }
}

void UdpServer::listen()
{
    memset(&m_cliaddr, 0, sizeof(m_cliaddr));
    uint len = sizeof(m_cliaddr); //len is value/result
    char buffer[MAXLINE];
    ssize_t n = recvfrom(m_sockfd, buffer, MAXLINE,
                                         MSG_WAITALL, ( struct sockaddr *) &m_cliaddr,
                                         &len);
    std::string packet = bufferToPacket(buffer);
    std::istringstream sBuf(packet, std::ios::binary);
    cereal::BinaryInputArchive iarchive(sBuf);

    char command;
    std::string username;
    iarchive(command, username);

    switch (command) {
    case '1':
    {
        std::cout << username << '\t' << inet_ntoa(m_cliaddr.sin_addr) << '\t' << m_cliaddr.sin_port << " detected\n";
        sendOldToNewUserList();
        sendChangesToOldClients(username);
        m_clientmap.insert ( std::pair<std::string,ClientData>(username,{inet_ntoa(m_cliaddr.sin_addr), m_cliaddr.sin_port}) );
        break;
    }
    case '0':
        m_clientmap.erase(username);
        break;
    default:
        break;
    }
    std::cout<<"map size "<<m_clientmap.size()<<'\n';
}
