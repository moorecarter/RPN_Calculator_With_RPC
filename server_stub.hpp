
// 1. Create a socket
// 2. Bind the socket to an interface and port
// 3. Begin Loop
// 4.
// Receive a message (using recvfrom)
// 5.
// Unmarshall the parameters (and metadata)
// 6.
// Verify magic, and version
// 7.
// Determine which function is called
// 8.
// Call the implementation of the function
// 9.
// Marshal the result (and metadata)
// 10.
// Send the reply to the sender of the received message
// 11.End Loop

//+
// File: serverStub.hpp
//
// Purpose: takes a protobuf string from a lower level and executes the command inside it.
//-

#include <arpa/inet.h>
#include <sys/socket.h>

#include "RPN.hpp"
#include "RPNMessage.pb.h"

#define GROUPNUM 8
#define PORT_LO (GROUPNUM * 100 + 2900)
#define PORT_HI (PORTLO + 100)
#define ADDR "127.0.0.1"

class RPNServerStub {
public:
    typedef enum {
        ServErrorNone = 0, SocketCreateError,
        BindError,
    } ServerError;
    class ServerException : public std::exception {
    private:
        std::string message;
    public:
        ServerException(const std::string& msg, ServerError code) : message(msg), errorCode(code) {}
        const ServerError errorCode;
        const char* what() const noexcept override {
            return message.c_str();
        }
    };

    RPNServerStub(uint16_t port);
    ~RPNServerStub();
    void spin();
    
private:
    
    
    static const uint32_t magic = 'E477';
    static const uint32_t version1x = 0x0100;
    
    void callMethodVersion1(RPNMessage::RPNMessageReq &receivedMsg, RPNMessage::RPNMessageResp &replyMsg);
    
    bool alive = false;
    RPNStack RPNStore;

    // network management
    int sockfd;
    struct sockaddr_in servaddr;
    in_port_t portnum;
    static const int MAXMSG = 2048;
    uint8_t udpMessage[MAXMSG];
    
};
