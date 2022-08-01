#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#define MAXLINE  (1024*1024)
std::string bufferToPacket(char buffer[MAXLINE])
{
    uint16_t * packetLengthPtr = reinterpret_cast<uint16_t *> (buffer); // make pointer to variable containing length
    uint16_t packetLength = * packetLengthPtr;                          // find length
//    std::cout<< "packetLength == " << packetLength<<std::endl;
    std::string packet{buffer+2, packetLength};                         // make string of length
    return packet;
}

//         std::string tmp = 'u' + key + ';' + val.getAddress() + ';' + std::to_string(val.getPort());

struct CmdClientinfo
{
    char command = 'u';
    std::string username;
    char delimeter = ';';
    std::string address;
    std::string port;

    template <typename Archive>
    void serialize(Archive& archive)
    {
        archive(command, username, delimeter, address, delimeter, port);
    }
};
