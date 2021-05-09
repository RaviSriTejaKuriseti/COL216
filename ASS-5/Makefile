CXX=g++
CXXFLAGS=-g -Wall -Wextra -c
#LINKER_FLAGS=
EXEC=a
OBJS=core.o finitequeue.o Memory.o main.o 

RM=rm

all:begin
default:begin

begin:core.o finitequeue.o Memory.o main.o 
	$(CXX) -o $(EXEC) $(OBJS)
core.o : core.cpp core.h auxiliary.h Memory.h finitequeue.h
	$(CXX) $(CXXFLAGS) core.cpp
finitequeue.o: finitequeue.cpp finitequeue.h
	$(CXX) $(CXXFLAGS) finitequeue.cpp
Memory.o: Memory.cpp Memory.h finitequeue.o
	$(CXX) $(CXXFLAGS) Memory.cpp
main.o: main.cpp core.o finitequeue.o Memory.o 
	$(CXX) $(CXXFLAGS) main.cpp

run:
	./$(EXEC)
clean:
	$(RM) -rf $(OBJS) $(EXEC)