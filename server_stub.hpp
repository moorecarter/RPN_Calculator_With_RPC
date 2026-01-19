#include <arpa/inet.h>
#include <sys/socket.h>
#include <memory>
#include <string>
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

    RPNServerStub(uint16_t port );
    ~RPNServerStub();
    void spin();
    
private:
    static const uint32_t magic = 'RPN8';
    static const uint32_t version1x = 0x0100;
    
    void callMethodVersion1(RPNMessage::RPNMessageReq &receivedMsg, RPNMessage::RPNMessageResp &replyMsg);
    
    bool alive = false;
    std::unique_ptr<RPNStack> RPNStore;

    // network management
    int sockfd;
    struct sockaddr_in servaddr;
    in_port_t portnum;
    static const int MAXMSG = 2048;
    uint8_t udpMessage[MAXMSG];
    
};
