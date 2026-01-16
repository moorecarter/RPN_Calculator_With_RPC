#include "RPN.hpp"
#include "RPNImplNet.hpp"

RPNStack::RPNStack() { pImpl = std::make_unique<RPNStack::RPNImpl>(); }

bool RPNStack::RPNPush(float value) { return pImpl->RPNPush(value); };
RPNStack::RPNValueResult RPNStack::RPNPop() { return pImpl->RPNPop(); };
RPNStack::RPNValueResult RPNStack::RPNRead() { return pImpl->RPNRead(); };
bool RPNStack::RPNSwap() { return pImpl->RPNSwap(); };

RPNStack::RPNValueResult RPNStack::add() {
  return pImpl->operation(RPNStack::opcode::Add);
};
RPNStack::RPNValueResult RPNStack::mult() {
  return pImpl->operation(RPNStack::opcode::Mult);
};
RPNStack::RPNValueResult RPNStack::sub() {
  return pImpl->operation(RPNStack::opcode::Sub);
};
RPNStack::RPNValueResult RPNStack::div() {
  return pImpl->operation(RPNStack::opcode::Div);
};