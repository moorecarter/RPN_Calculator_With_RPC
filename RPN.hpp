#ifndef __RPN_HPP__
#define __RPN_HPP__



struct RPNValueResult{
    bool status;
    float value;
};
enum opcode {Add, Sub, Mult, Div};

class RPNStack{
    private:
        static const int SIZE = 4; 
        int length;
        float data[SIZE];
    public:
        RPNStack();
        bool RPNPush(float value);
        RPNValueResult RPNPop();
        RPNValueResult RPNRead();
        bool RPNSwap();
        RPNValueResult operation(int op);
        void RPNPrintData();
        //can later remove
        RPNValueResult add();
        RPNValueResult mult();
        RPNValueResult sub();
        RPNValueResult div();
        
       
};







#endif