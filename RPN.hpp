#ifndef __RPN_HPP__
#define __RPN_HPP__

#include <memory>

class RPNStack {
 private:
  class RPNImpl;
  std::unique_ptr<RPNImpl> pImpl;

 public:
  struct RPNValueResult {
    bool status;
    float value;
  };
  enum opcode { Nop, Add, Sub, Mult, Div };
  RPNStack();
  ~RPNStack();
  bool RPNPush(float value);
  RPNValueResult RPNPop();
  RPNValueResult RPNRead();
  bool RPNSwap();
  RPNValueResult operation(opcode op);
  void RPNPrintData();

  // for client stub only
  RPNValueResult add();
  RPNValueResult mult();
  RPNValueResult sub();
  RPNValueResult div();
};

#endif