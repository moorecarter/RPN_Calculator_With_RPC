
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

SERVER_OBJS=server_main.o kv.o KVServerStub.o E477KV.pb.o
CLIENT_OBJS=client_main.o kvClientStub.o E477KV.pb.o KVImplNet.o
SINGLE_OBJS=client_main.o kv.o

all: bin/kvServerClassProto bin/kvClientClassProto bin/kvSingleClassProto

bin/kvSingleClassProto:  $(SINGLE_OBJS)
	$(CXX) -g -o bin/kvSingleClassProto $(SINGLE_OBJS) $(LDFLAGS)
bin/kvServerClassProto:  $(SERVER_OBJS)
	$(CXX) -g -o bin/kvServerClassProto $(SERVER_OBJS) $(LDFLAGS)
bin/kvClientClassProto:  $(CLIENT_OBJS)
	$(CXX) -g -o bin/kvClientClassProto $(CLIENT_OBJS) $(LDFLAGS)
	
server_main.o:  KVServerStub.hpp E477KV.pb.h
serverStub.o:  kv.hpp E477KV.pb.h
client_main.o:  kv.hpp
kv.o: kv.hpp
kvClientStub.o: kv.hpp E477KV.pb.h
KVServerStub.o: kv.hpp E477KV.pb.h KVServerStub.hpp


E477KV.pb.h E477KV.pb.cc: E477KV.proto
	protoc --cpp_out=. E477KV.proto
	
clean:
	rm bin/* *.o  E477KV.pb.h E477KV.pb.cc
