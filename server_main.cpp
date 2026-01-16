#include <iostream>

using namespace std;
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
// File: serverStub.cpp
//
// Purpose: takes a protobuf string from a lower level and executes the command inside it.
//-

#include <cstring>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <atomic>


#include "server_stub.hpp"

#ifdef __APPLE__
#define MSG_CONFIRM 0
#endif

using namespace std;

RPNServerStub::RPNServerStub(uint16_t port): portnum(port){
    
    // get a socket to recieve messges
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        throw ServerException("socket creation failed",SocketCreateError);
    }
    
    // set port to listen to (on any interface)
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(portnum);
    
    if (::bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
        throw ServerException("bind failed",BindError);
    }
}

void KVServerStub::spin(){
    alive = true;
    
    socklen_t len;
    int n;
    struct sockaddr_in cliaddr;
    
    while(alive){
        cerr << "waiting for call from client" << endl;

        // wait for a mesage from a client
        memset(&cliaddr, 0, sizeof(cliaddr));

        len = sizeof(cliaddr);  //len is value/result
        n = recvfrom(sockfd, (uint8_t *)udpMessage, MAXMSG,
            MSG_WAITALL, ( struct sockaddr *) &cliaddr,
            &len);
        cerr << "server received " << n << " bytes." << endl;

        RPNMessage::RPNMessageReq receivedMsg;
        RPNMessage::RPNMessageResp replyMsg;


        if (!receivedMsg.ParseFromArray(udpMessage,n)){
            cerr << "Couild not parse message" << endl;
            // ignore
        } else {
            
            cerr << "message parsed" << endl;

            if (receivedMsg.magic() != magic){
                std::cerr << "service unrecognized message" << std::endl;
            } else {
                // start by copying version and serial to reply
                replyMsg.set_magic(magic);
                replyMsg.set_version(receivedMsg.version());
                replyMsg.set_serial(receivedMsg.serial());
                
                if ((receivedMsg.version() & 0xFF00)  == version1x){
                    // dispatch version 1.x
                    callMethodVersion1(receivedMsg, replyMsg);
                } else {
                    std::cerr << "unrecognized version" << std::endl;
                    // for now ignore wong version number.
                }
                // at this point in time the reply is complete
                // send response back
                uint32_t msglen = replyMsg.ByteSizeLong();
                // double check size
                replyMsg.SerializeToArray(udpMessage, msglen);
                //cerr << "reply message" << HexDump{udpMessage,msglen} ;
                
                int servern = sendto(sockfd, udpMessage, msglen,
                                     MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
                //cerr << "server sent " << servern << " bytes" << endl;
            }
        }
    }
}

RNPServerStub::~RNPServerStub(){
    alive = false;
    close(sockfd);
}


void RNPServerStub::callMethodVersion1(RNPMessage::RPNMessageReq &receivedMsg, RNPMessage::RPNMessageResp &replyMsg){
        if (receivedMsg.has_putargs()){
            cerr << "put message requested" << endl;

            const RNPMessage::RPNMessageReq& pushreq = receivedMsg.push_req();

            int key = pushreq.key();
            string valueAsStr = preq.value();

            bool putRes = kv->put(key, valueAsStr);

            E477KV::putResponse *presp = replyMsg.mutable_putres();
            presp->set_status(putRes);
        }
        if (receivedMsg.has_getargs()){
            cerr << "get message requested" << endl;

            const E477KV::getRequest& greq = receivedMsg.getargs();

            int key = greq.key();

            KV::GetResult result = kv->get(key);

            E477KV::getResponse *gr = replyMsg.mutable_getres();
            gr->set_status(result.status);
            gr->set_value(result.value);
        }
}
