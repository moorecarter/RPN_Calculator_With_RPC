//+
// File: kvClientStub
//
// Purpose: Provides network transport for the kvAPI
//-
#include <cstring>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <atomic>


#include "server_stub.hpp"

const uint16_t KV_START_PORT = 3000;
const uint16_t KV_END_PORT = 3499;


int main(int argc, char * argv[]){
    if (argc != 2){
        std::cerr << "Usage: " << argv[0] << " portnumber" << std::endl;
        exit(1);
    }
    uint16_t port = atoi(argv[1]);
    if (port == 0){
        std::cerr << "Port number canot be zero" << std::endl;
        exit(1);
    }
    if (port < KV_START_PORT || port > KV_END_PORT){
        std::cerr << "Port number must be between "
        << KV_START_PORT << " and "
        << KV_END_PORT << std::endl;
        exit(1);
    }
    KVServerStub stub(port);
    stub.spin();
    return 0;
}
