#include "RPN.hpp"

#include <iostream>

class RPNStack::RPNImpl {
 public:
  static const int SIZE = 4;
  int length;
  float data[SIZE];
};

RPNStack::RPNStack() : pImpl(std::make_unique<RPNStack::RPNImpl>()) {};

bool RPNStack::RPNPush(float value) {
  for (int i = pImpl->SIZE - 1; i > 0; i--) {
    pImpl->data[i] = pImpl->data[i - 1];
  }
  pImpl->data[0] = value;
  pImpl->length++;
  return true;
}

RPNValueResult RPNStack::RPNPop() {
  if (pImpl->length == 0) {
    return {false, 0.0};
  }
  float retData = pImpl->data[0];
  // shift elements to pop
  for (int i = 0; i < pImpl->SIZE - 1; i++) {
    pImpl->data[i] = pImpl->data[i + 1];
  }
  return {true, retData};
}

RPNValueResult RPNStack::RPNRead() {
  if (pImpl->length == 0) {
    return {false, 0.0};
  }
  return {true, pImpl->data[0]};
}

bool RPNStack::RPNSwap() {
  float temp = pImpl->data[0];
  pImpl->data[0] = pImpl->data[1];
  pImpl->data[1] = temp;
  return true;
}

RPNValueResult RPNStack::operation(int op) {
  RPNValueResult res1 = RPNStack::RPNPop();
  if (!res1.status) {
    return {false, 0.0};
  }
  RPNValueResult res2 = RPNStack::RPNPop();
  if (!res2.status) {
    return {false, 0.0};
  }
  float ans;
  switch (op) {
    case (Add):
      ans = res2.value + res1.value;
      break;
    case (Sub):
      ans = res2.value - res1.value;
      break;

    case (Mult):
      ans = res2.value * res1.value;
      break;
    case (Div):
      ans = res2.value / res1.value;
      break;
    default:
      return {false, 0.0};
  }

  return {RPNStack::RPNPush(ans), ans};
}

void RPNStack::RPNPrintData() {
  for (int i = 0; i < pImpl->SIZE - 1; i++) {
    std::cout << pImpl->data[i] << ",";
  }
  std::cout << pImpl->data[pImpl->SIZE - 1] << std::endl;
}

RPNValueResult RPNStack::add() {
  RPNValueResult res1 = RPNStack::RPNPop();
  if (!res1.status) {
    return {false, 0.0};
  }
  RPNValueResult res2 = RPNStack::RPNPop();
  if (!res2.status) {
    return {false, 0.0};
  }
  float ans = res2.value + res1.value;
  return {RPNStack::RPNPush(ans), ans};
}
RPNValueResult RPNStack::mult() {
  RPNValueResult res1 = RPNStack::RPNPop();
  if (!res1.status) {
    return {false, 0.0};
  }
  RPNValueResult res2 = RPNStack::RPNPop();
  if (!res2.status) {
    return {false, 0.0};
  }
  float ans = res2.value * res1.value;
  return {RPNStack::RPNPush(ans), ans};
}
RPNValueResult RPNStack::sub() {
  RPNValueResult res1 = RPNStack::RPNPop();
  if (!res1.status) {
    return {false, 0.0};
  }
  RPNValueResult res2 = RPNStack::RPNPop();
  if (!res2.status) {
    return {false, 0.0};
  }
  float ans = res2.value - res1.value;
  return {RPNStack::RPNPush(ans), ans};
}
RPNValueResult RPNStack::div() {
  RPNValueResult res1 = RPNStack::RPNPop();
  if (!res1.status) {
    return {false, 0.0};
  }
  RPNValueResult res2 = RPNStack::RPNPop();
  if (!res2.status) {
    return {false, 0.0};
  }
  float ans = res2.value / res1.value;
  return {RPNStack::RPNPush(ans), ans};
}
