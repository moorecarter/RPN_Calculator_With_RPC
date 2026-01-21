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

RPNServerStub::RPNServerStub(uint16_t port): portnum(port), RPNStore(std::make_unique<RPNStack>()){
    
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

void RPNServerStub::spin(){
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

RPNServerStub::~RPNServerStub(){
    alive = false;
    close(sockfd);
}

void RPNServerStub::callMethodVersion1(RPNMessage::RPNMessageReq &receivedMsg, RPNMessage::RPNMessageResp &replyMsg){
        if (receivedMsg.has_push_req()){
            cerr << "push message requested" << endl;

            const RPNMessage::RPNPushReq& pushreq = receivedMsg.push_req();
            float value = pushreq.value();
            
            bool pushRes = RPNStore->RPNPush(value);

            RPNMessage::RPNPushResp *pushresp = replyMsg.mutable_push_resp();
            pushresp->set_status(pushRes);
        }
        else if (receivedMsg.has_pop_req()){
            cerr << "pop message requested" << endl;
            RPNStack::RPNValueResult popRes = RPNStore->RPNPop();

            RPNMessage::RPNPopResp *popresp = replyMsg.mutable_pop_resp();
            popresp->set_status(popRes.status);
            popresp->set_value(popRes.value);
        }
        else if (receivedMsg.has_read_req()){
            cerr << "read message requested" << endl;

            RPNStack::RPNValueResult readRes = RPNStore->RPNRead();

            RPNMessage::RPNReadResp *readresp = replyMsg.mutable_read_resp();
            readresp->set_status(readRes.status);
            readresp->set_value(readRes.value);
        }
        else if (receivedMsg.has_swap_req()){
            cerr << "swap message requested" << endl;

            bool swapRes = RPNStore->RPNSwap();

            RPNMessage::RPNSwapResp *swapresp = replyMsg.mutable_swap_resp();
            swapresp->set_status(swapRes);
        }
        else if (receivedMsg.has_op_req()){
            cerr << "operator message requested" << endl;

            const RPNMessage::RPNOperatorReq& opreq = receivedMsg.op_req();   
            const RPNMessage::Operator op = opreq.op();
            // translate to correct typing
            RPNStack::opcode opcode;
            switch (op) {
                case RPNMessage::NOP:
                    opcode = RPNStack::opcode::Nop;
                    break;
                case RPNMessage::Add:
                    opcode = RPNStack::opcode::Add;
                    break;
                case RPNMessage::Sub:
                    opcode = RPNStack::opcode::Sub;
                    break;
                case RPNMessage::Mult:
                    opcode = RPNStack::opcode::Mult;
                    break;
                case RPNMessage::Div:
                    opcode = RPNStack::opcode::Div;
                    break;
                default:
                    throw std::invalid_argument("Invalid RPNMessage::Operator");
            }
            RPNStack::RPNValueResult opRes = RPNStore->operation(opcode);

            RPNMessage::RPNOperatorResp *opresp = replyMsg.mutable_op_resp();
            opresp->set_status(opRes.status);
            opresp->set_value(opRes.value);
        }
 
}
