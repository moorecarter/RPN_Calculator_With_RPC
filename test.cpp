#include "RPN.hpp"
#include <iostream>

using namespace std;

int main(){
    RPNStack c = RPNStack();
    c.RPNPush(5.0);
    c.RPNPush(7.0);
    c.RPNPrintData();
    cout << c.RPNRead().value << endl;
    c.operation(Add);
    cout << c.RPNRead().value << endl;
    c.RPNPush(10.0);
    c.RPNPrintData();
    cout << c.RPNRead().value << endl;
    c.operation(Mult);
    cout << c.RPNRead().value << endl;
        c.RPNPush(5.0);
    c.RPNPush(7.0);
        c.RPNPush(6.0);
    c.RPNPrintData();
    cout << c.RPNPop().value << endl;
    c.RPNPrintData();
    c.RPNPop();
    c.RPNPrintData();
        c.RPNPop();
    c.RPNPrintData();
    c.RPNPush(100.0);
    c.RPNPrintData();
    c.RPNSwap();
    c.RPNPrintData();

}