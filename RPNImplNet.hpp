#ifndef KVImpNet_hpp
#define KVImpNet_hpp

#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <atomic>
#include "RPN.hpp"

class RPNStack::RPNImpl {
 public:
  RPNImpl();
  ~RPNImpl();
  bool RPNPush(float value);
  RPNStack::RPNValueResult RPNPop();
  RPNStack::RPNValueResult RPNRead();
  bool RPNSwap();
  RPNStack::RPNValueResult operation(opcode op);
  void RPNPrintData();

 private:
  int sockfd;
  const uint32_t serverAddrNumber = 0x7F000001;  // 127.0.0.1
  struct sockaddr_in servaddr;
  static const uint32_t magic = 'RPN8';
  static const uint32_t version1x = 0x0100;
  static const unsigned short RPN_START_PORT = 3700;
  static const unsigned short RPN_END_PORT = 3799;
  unsigned short port;
  std::atomic<uint32_t> serial;
  static const int maxMesg = 1400;
  uint8_t buffer[maxMesg];  // to serialize into
};

#endif