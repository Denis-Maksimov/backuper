CC:=gcc
CXX:=g++

SRC=$(wildcard *.cpp)

DEPS=
LIBS+= stdc++fs 
#LIBS+= sqlite3 curses tvision

CXXFLAGS+=-O2 -Wall -std=c++17 -g
# -I./ $(shell pkg-config $(DEPS) --cflags)

# LDFLAGS+= $(shell pkg-config $(DEPS) --libs)
LDFLAGS+= $(addprefix -l,$(LIBS))

OBJ=$(SRC:%.cpp=%.o)

EXEC:=./restore_me

all: $(EXEC)

%.o: %.c
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

