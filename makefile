
CC:=gcc
CXX:=g++

SRC=$(wildcard *.cpp)

DEPS=
LIBS=stdc++fs

CXXFLAGS+=-O2 -Wall -std=c++17
# -I./ $(shell pkg-config $(DEPS) --cflags)

LDFLAGS:=$(shell pkg-config $(DEPS) --libs) $(-l$(LIBS))

OBJ=$(SRC:%.cpp=%.o)

EXEC:=./restore_me

all: $(EXEC)

%.o: %.c
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(EXEC): $(OBJ)
 	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)


#g++ ./backuper.cpp -std=c++17 -lstdc++fs
