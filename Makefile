
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

SERVER_OBJS=server_main.o RPN.o server_stub.o RPN.pb.o
CLIENT_OBJS=client_main.o client_stub.o RPN.pb.o RPNImplNet.o
SINGLE_OBJS=client_main.o RPN.o

all: bin/RPNServerClassProto bin/RPNClientClassProto bin/RPNSingleClassProto

bin/RPNSingleClassProto:  $(SINGLE_OBJS)
	$(CXX) -g -o bin/RPNSingleClassProto $(SINGLE_OBJS) $(LDFLAGS)
bin/RPNServerClassProto:  $(SERVER_OBJS)
	$(CXX) -g -o bin/RPNServerClassProto $(SERVER_OBJS) $(LDFLAGS)
bin/RPNClientClassProto:  $(CLIENT_OBJS)
	$(CXX) -g -o bin/RPNClientClassProto $(CLIENT_OBJS) $(LDFLAGS)
	
server_main.o:  server_stub.hpp RPN.pb.h
server_stub.o:  RPN.hpp RPN.pb.h
client_main.o:  RPN.hpp
RPN.o: RPN.hpp
client_stub.o: RPN.hpp RPN.pb.h
server_stub.o: RPN.hpp RPN.pb.h server_stub.hpp


RPN.pb.h RPN.pb.cc: RPN.proto
	protoc --cpp_out=. RPN.proto
	
clean:
	rm bin/* *.o  RPN.pb.h RPN.pb.cc
