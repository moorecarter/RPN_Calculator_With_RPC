#include <iostream>

#include "RPN.hpp"

using namespace std;

// 1. Create a socket
// 2. Increment the serial number
// 3. Marshal the metadata and parameters
// (including which function is being called)
// 4. Send message to server
// 5. Begin Loop
// 6.
// Receive a message (using recvfrom)
// 7.
// Unmarshall the potential result (and metadata)
// 8.
// Verify magic and version
// 9.
// Verify serial number (back to 5 if incorrect)
// 10.
// use the result
// 11.End Loop

int main(int argc, char * argv[]){
    RPNStack rpn;
    bool status;
    RPNStack::RPNValueResult result;

    //Test Push and Read
    cout << "Push 5" << endl;
    if(!rpn.RPNPush(5)){
        cerr << "Failed to Push" << endl;
    }

    result = rpn.RPNRead();
    cout << "Read Stack Result: " << result.value << endl;   
    if(!result.status){
        cerr << "Failed to Read" << endl;
    }

    //Test Swap
    cout << "Swap" << endl;
    if(!rpn.RPNSwap()){
        cerr << "Failed to Swap" << endl;
    }
    result = rpn.RPNRead();
    cout << "Read Stack Result: " << result.value << endl;   
    if(!result.status){
        cerr << "Failed to Read" << endl;
    }
    cout << "Swap Back" << endl;
    if(!rpn.RPNSwap()){
        cerr << "Failed to Swap" << endl;
    }
    result = rpn.RPNRead();
    cout << "Read Stack Result: " << result.value << endl;   
    if(!result.status){
        cerr << "Failed to Read" << endl;
    }
    
    //Test Add
    cout << "Push 2.5 and Add" << endl;
    if(!rpn.RPNPush(2.5)){
        cerr << "Failed to Push" << endl;
    }
    result = rpn.add();
    if(!result.status){
        cerr << "Failed to Add" << endl;
    }
    result = rpn.RPNRead();
    cout << "Read Stack Result: " << result.value << endl;   
    if(!result.status){
        cerr << "Failed to Read" << endl;
    }

    //Test Subtract
    cout << "Push 6 and Subtract" << endl;
    if(!rpn.RPNPush(6)){
        cerr << "Failed to Push" << endl;
    }
    result = rpn.sub();
    if(!result.status){
        cerr << "Failed to Subtract" << endl;
    }
    result = rpn.RPNRead();
    cout << "Read Stack Result: " << result.value << endl;   
    if(!result.status){
        cerr << "Failed to Read" << endl;
    }

    //Test Multiply
    cout << "Push 3 and Multiply" << endl;
    if(!rpn.RPNPush(3)){
        cerr << "Failed to Push" << endl;
    }
    result = rpn.mult();
    if(!result.status){
        cerr << "Failed to Multiply" << endl;
    }
    result = rpn.RPNRead();
    cout << "Read Stack Result: " << result.value << endl;   
    if(!result.status){
        cerr << "Failed to Read" << endl;
    }

    //Test Divide
    cout << "Push 2 and Divide" << endl;
    if(!rpn.RPNPush(2)){
        cerr << "Failed to Push" << endl;
    }
    result = rpn.div();
    if(!result.status){
        cerr << "Failed to Divide" << endl;
    }
    result = rpn.RPNRead();
    cout << "Read Stack result: " << result.value << endl;   
    if(!result.status){
        cerr << "Failed to Read" << endl;
    }

    //Push four values to ensure push works correctly
    cout << "Push 2.5" << endl;
    if(!rpn.RPNPush(2.5)){
        cerr << "Failed to Push" << endl;
    }

    cout << "Push 7" << endl;
    if(!rpn.RPNPush(7)){
        cerr << "Failed to Push" << endl;
    }

    cout << "Push 2" << endl;
    if(!rpn.RPNPush(2)){
        cerr << "Failed to Push" << endl;
    }

    //Pop and read to show correctness


}