
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CXX=g++ -I /usr/local/include
CXXFLAGS=-std=c++2a -g -pthread -g
LDFLAGS=-lprotobuf  -pthread
endif
ifeq ($(UNAME_S),Darwin)
CXXFLAGS=-std=c++20 -g -pthread -g
CXX=c++ -I /usr/local/include
LDFLAGS=-lprotobuf  -pthread
endif

SERVER_OBJS=server_main.o RPN.o server_stub.o RPNMessage.pb.o
CLIENT_OBJS=client_main.o client_stub.o RPNMessage.pb.o RPNImplNet.o
SINGLE_OBJS=client_main.o RPN.o

all: bin/RPNServer bin/RPNClient bin/RPNSingle

bin/RPNSingle:  $(SINGLE_OBJS)
	$(CXX) -g -o bin/RPNSingle $(SINGLE_OBJS) $(LDFLAGS)
bin/RPNServer:  $(SERVER_OBJS)
	$(CXX) -g -o bin/RPNServer $(SERVER_OBJS) $(LDFLAGS)
bin/RPNClient:  $(CLIENT_OBJS)
	$(CXX) -g -o bin/RPNClient $(CLIENT_OBJS) $(LDFLAGS)
	
server_main.o:  server_stub.hpp RPNMessage.pb.h
server_stub.o:  RPN.hpp RPNMessage.pb.h
client_main.o:  RPN.hpp
RPN.o: RPN.hpp
client_stub.o: RPN.hpp RPNMessage.pb.h
server_stub.o: RPN.hpp RPNMessage.pb.h server_stub.hpp


RPNMessage.pb.h RPNMessage.pb.cc: RPNMessage.proto
	protoc --cpp_out=. RPNMessage.proto
	
clean:
	rm bin/* *.o  RPNMessage.pb.h RPNMessage.pb.cc
