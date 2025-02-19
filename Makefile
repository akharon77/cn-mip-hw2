CXX = g++
CXX_FLAGS = -std=c++2a

SRC  = $(wildcard *.cpp)
OBJ  = $(patsubst %.cpp,%.o,$(SRC))
DEP  = $(patsubst %.cpp,%.d,$(SRC))
BIN  = $(wildcard *.out)

all: client_tcp.out client_udp.out server_tcp.out server_udp.out

client_tcp.out: client_tcp.o tcp.o utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@

client_udp.out: client_udp.o udp.o utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@

server_tcp.out: server_tcp.o tcp.o utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@

server_udp.out: server_udp.o udp.o utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@

-include $(DEP)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

clean:
	rm -rf $(DEP) $(OBJ) $(BIN)
