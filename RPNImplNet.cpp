#include "RPNImplNet.hpp"

#include "RPNMessage.pb.h"

using namespace std;

RPNStack::RPNImpl::RPNImpl() {
  char *portStr = getenv("PORT");
  if (portStr == NULL) {
    cerr << "Must set port number in environment" << endl;
    exit(1);
  }
  uint16_t port = atoi(portStr);
  if (port < RPN_START_PORT || port > RPN_END_PORT) {
    cerr << "Port number must be between " << RPN_START_PORT << " and "
         << RPN_END_PORT << endl;
    exit(1);
  }
  cout << "Client using port " << port << endl;

  memset(&(servaddr), 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = htonl(serverAddrNumber);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    cerr << "Socket creation failed" << endl;
    exit(1);
  }

  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    cerr << "Timeout" << endl;
    exit(1);
  }
}

RPNStack::RPNImpl::~RPNImpl() { close(sockfd); }

//+
// RPNPush method
//
// stub for RPNPush routine. Sends a RPNPush message to server and
// waits for reply.
//-
bool RPNStack::RPNImpl::RPNPush(float value) {
  int n;
  socklen_t len;
  uint32_t blen = maxMesg;
  struct sockaddr_in servaddrreply;

  uint32_t msgSerial = serial++;

  // marshal parameters to send.
  RPNMessage::RPNMessageReq msg;
  msg.set_magic(magic);
  msg.set_version(version1x);
  msg.set_serial(msgSerial);
  RPNMessage::RPNPushReq *pr = msg.mutable_push_req();
  pr->set_value(value);

  blen = msg.ByteSizeLong();
  if (blen > maxMesg) {
    cerr << " *** msg too long" << endl;
    return false;
  }
  msg.SerializeToArray(buffer, blen);

  n = sendto(sockfd, (const char *)buffer, blen, 0,
             (const struct sockaddr *)&servaddr, sizeof(servaddr));

  RPNMessage::RPNMessageResp pushRespMsg;
  bool gotMessage = true;
  do {
    len = sizeof(struct sockaddr_in);
    n = recvfrom(sockfd, (char *)buffer, maxMesg, MSG_WAITALL,
                 (struct sockaddr *)&servaddrreply, &len);

    // check for timeout here..
    if (n == -1) {
      return false;
    };

    if (!pushRespMsg.ParseFromArray(buffer, n)) {
      cerr << "Could not parse message" << endl;
      // wait for another mesage
      gotMessage = false;
    } else {
      if (pushRespMsg.magic() != 'RPN8') {
        cerr << "Magic Mismatch" << endl;
        gotMessage = false;
      } else {
        if (msg.version() != pushRespMsg.version()) {
          cerr << "Version Mismatch" << endl;
          gotMessage = false;
        } else {
          // wait for another message is the serial number is wrong.
          if (msg.serial() != pushRespMsg.serial()) {
            cerr << "Serial Numbers Mismatch" << endl;
            gotMessage = false;
          }
        }
      }
    }
  } while (!gotMessage);

  bool returnRes = false;
  if (pushRespMsg.has_push_resp()) {
    returnRes = pushRespMsg.push_resp().status();
  } else {
    cerr << "wrong message type" << endl;
    returnRes = false;
  }

  return returnRes;
}

//+
// RPNPop method
//
// stub for RPNPop routine. Sends a RPNPop message to server and
// waits for reply.
//-
RPNStack::RPNValueResult RPNStack::RPNImpl::RPNPop() {
  int n;
  socklen_t len;
  uint32_t blen = maxMesg;
  struct sockaddr_in servaddrreply;
  RPNValueResult retVal;

  uint32_t msgSerial = serial++;

  // marshal parameters to send.
  RPNMessage::RPNMessageReq msg;
  msg.set_magic(magic);
  msg.set_version(version1x);
  msg.set_serial(msgSerial);
  RPNMessage::RPNPopReq *pr = msg.mutable_pop_req();

  blen = msg.ByteSizeLong();
  if (blen > maxMesg) {
    cerr << " *** msg too long" << endl;
    retVal.status = false;
    return retVal;
  }
  msg.SerializeToArray(buffer, blen);

  n = sendto(sockfd, (const char *)buffer, blen, 0,
             (const struct sockaddr *)&servaddr, sizeof(servaddr));

  RPNMessage::RPNMessageResp popRespMsg;
  bool gotMessage = true;
  do {
    len = sizeof(struct sockaddr_in);
    n = recvfrom(sockfd, (char *)buffer, maxMesg, MSG_WAITALL,
                 (struct sockaddr *)&servaddrreply, &len);

    // check for timeout here..
    if (n == -1) {
      retVal.status = false;
      return retVal;
    };

    if (!popRespMsg.ParseFromArray(buffer, n)) {
      cerr << "Could not parse message" << endl;
      // wait for another mesage
      gotMessage = false;
    } else {
      if (popRespMsg.magic() != 'RPN8') {
        cerr << "Magic Mismatch" << endl;
        gotMessage = false;
      } else {
        if (msg.version() != popRespMsg.version()) {
          cerr << "Version Mismatch" << endl;
          gotMessage = false;
        } else {
          // wait for another message is the serial number is wrong.
          if (msg.serial() != popRespMsg.serial()) {
            cerr << "Serial Numbers Mismatch" << endl;
            gotMessage = false;
          }
        }
      }
    }
  } while (!gotMessage);

  if (popRespMsg.has_pop_resp()) {
    retVal.status = popRespMsg.pop_resp().status();
    retVal.value = popRespMsg.pop_resp().value();
  } else {
    cerr << "wrong message type" << endl;
    retVal.status = false;
  }

  return retVal;
}

//+
// RPNRead method
//
// stub for RPNRead routine. Sends a RPNRead message to server and
// waits for reply.
//-
RPNStack::RPNValueResult RPNStack::RPNImpl::RPNRead() {
  int n;
  socklen_t len;
  uint32_t blen = maxMesg;
  struct sockaddr_in servaddrreply;
  RPNValueResult retVal;

  uint32_t msgSerial = serial++;

  // marshal parameters to send.
  RPNMessage::RPNMessageReq msg;
  msg.set_magic(magic);
  msg.set_version(version1x);
  msg.set_serial(msgSerial);
  RPNMessage::RPNReadReq *pr = msg.mutable_read_req();

  blen = msg.ByteSizeLong();
  if (blen > maxMesg) {
    cerr << " *** msg too long" << endl;
    retVal.status = false;
    return retVal;
  }
  msg.SerializeToArray(buffer, blen);

  n = sendto(sockfd, (const char *)buffer, blen, 0,
             (const struct sockaddr *)&servaddr, sizeof(servaddr));

  RPNMessage::RPNMessageResp readRespMsg;
  bool gotMessage = true;
  do {
    len = sizeof(struct sockaddr_in);
    n = recvfrom(sockfd, (char *)buffer, maxMesg, MSG_WAITALL,
                 (struct sockaddr *)&servaddrreply, &len);

    // check for timeout here..
    if (n == -1) {
      retVal.status = false;
      return retVal;
    };

    if (!readRespMsg.ParseFromArray(buffer, n)) {
      cerr << "Could not parse message" << endl;
      // wait for another mesage
      gotMessage = false;
    } else {
      if (readRespMsg.magic() != 'RPN8') {
        cerr << "Magic Mismatch" << endl;
        gotMessage = false;
      } else {
        if (msg.version() != readRespMsg.version()) {
          cerr << "Version Mismatch" << endl;
          gotMessage = false;
        } else {
          // wait for another message is the serial number is wrong.
          if (msg.serial() != readRespMsg.serial()) {
            cerr << "Serial Numbers Mismatch" << endl;
            gotMessage = false;
          }
        }
      }
    }
  } while (!gotMessage);

  if (readRespMsg.has_read_resp()) {
    retVal.status = readRespMsg.read_resp().status();
    retVal.value = readRespMsg.read_resp().value();
  } else {
    cerr << "wrong message type" << endl;
    retVal.status = false;
  }

  return retVal;
}

//+
// RPNSwap method
//
// stub for RPNSwap routine. Sends a RPNSwap message to server and
// waits for reply.
//-
bool RPNStack::RPNImpl::RPNSwap() {
  int n;
  socklen_t len;
  uint32_t blen = maxMesg;
  struct sockaddr_in servaddrreply;

  uint32_t msgSerial = serial++;

  // marshal parameters to send.
  RPNMessage::RPNMessageReq msg;
  msg.set_magic(magic);
  msg.set_version(version1x);
  msg.set_serial(msgSerial);
  RPNMessage::RPNSwapReq *pr = msg.mutable_swap_req();

  blen = msg.ByteSizeLong();
  if (blen > maxMesg) {
    cerr << " *** msg too long" << endl;
    return false;
  }
  msg.SerializeToArray(buffer, blen);

  n = sendto(sockfd, (const char *)buffer, blen, 0,
             (const struct sockaddr *)&servaddr, sizeof(servaddr));

  RPNMessage::RPNMessageResp swapRespMsg;
  bool gotMessage = true;
  do {
    len = sizeof(struct sockaddr_in);
    n = recvfrom(sockfd, (char *)buffer, maxMesg, MSG_WAITALL,
                 (struct sockaddr *)&servaddrreply, &len);

    // check for timeout here..
    if (n == -1) {
      return false;
    };

    if (!swapRespMsg.ParseFromArray(buffer, n)) {
      cerr << "Could not parse message" << endl;
      // wait for another mesage
      gotMessage = false;
    } else {
      if (swapRespMsg.magic() != 'RPN8') {
        cerr << "Magic Mismatch" << endl;
        gotMessage = false;
      } else {
        if (msg.version() != swapRespMsg.version()) {
          cerr << "Version Mismatch" << endl;
          gotMessage = false;
        } else {
          // wait for another message is the serial number is wrong.
          if (msg.serial() != swapRespMsg.serial()) {
            cerr << "Serial Numbers Mismatch" << endl;
            gotMessage = false;
          }
        }
      }
    }
  } while (!gotMessage);

  bool returnRes = false;
  if (swapRespMsg.has_swap_resp()) {
    returnRes = swapRespMsg.swap_resp().status();
  } else {
    cerr << "wrong message type" << endl;
    returnRes = false;
  }

  return returnRes;
}

//+
// operation method
//
// stub for operation routine. Sends a operation message to server and
// waits for reply.
//-
RPNStack::RPNValueResult RPNStack::RPNImpl::operation(opcode op) {
  int n;
  socklen_t len;
  uint32_t blen = maxMesg;
  struct sockaddr_in servaddrreply;
  RPNValueResult retVal;

  uint32_t msgSerial = serial++;

  // marshal parameters to send.
  RPNMessage::RPNMessageReq msg;
  msg.set_magic(magic);
  msg.set_version(version1x);
  msg.set_serial(msgSerial);
  RPNMessage::RPNOperatorReq *pr = msg.mutable_op_req();
  pr->set_op((RPNMessage::Operator)op);

  blen = msg.ByteSizeLong();
  if (blen > maxMesg) {
    cerr << " *** msg too long" << endl;
    retVal.status = false;
    return retVal;
  }
  msg.SerializeToArray(buffer, blen);

  n = sendto(sockfd, (const char *)buffer, blen, 0,
             (const struct sockaddr *)&servaddr, sizeof(servaddr));

  RPNMessage::RPNMessageResp readRespMsg;
  bool gotMessage = true;
  do {
    len = sizeof(struct sockaddr_in);
    n = recvfrom(sockfd, (char *)buffer, maxMesg, MSG_WAITALL,
                 (struct sockaddr *)&servaddrreply, &len);

    // check for timeout here..
    if (n == -1) {
      retVal.status = false;
      return retVal;
    };

    if (!readRespMsg.ParseFromArray(buffer, n)) {
      cerr << "Could not parse message" << endl;
      // wait for another mesage
      gotMessage = false;
    } else {
      if (readRespMsg.magic() != 'RPN8') {
        cerr << "Magic Mismatch" << endl;
        gotMessage = false;
      } else {
        if (msg.version() != readRespMsg.version()) {
          cerr << "Version Mismatch" << endl;
          gotMessage = false;
        } else {
          // wait for another message is the serial number is wrong.
          if (msg.serial() != readRespMsg.serial()) {
            cerr << "Serial Numbers Mismatch" << endl;
            gotMessage = false;
          }
        }
      }
    }
  } while (!gotMessage);

  if (readRespMsg.has_read_resp()) {
    retVal.status = readRespMsg.read_resp().status();
    retVal.value = readRespMsg.read_resp().value();
  } else {
    cerr << "wrong message type" << endl;
    retVal.status = false;
  }

  return retVal;
}
