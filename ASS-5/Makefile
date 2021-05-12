CXX=g++
CXXFLAGS=-g -Wall -Wextra -c
#LINKER_FLAGS=
EXEC=a
OBJS=core.o finitequeue.o Memory.o main.o 

RM=rm

all:begin
default:begin

begin:finitequeue.o Memory.o core.o main.o 
	$(CXX) -o $(EXEC) $(OBJS)
finitequeue.o: 
	$(CXX) $(CXXFLAGS) finitequeue.cpp
Memory.o: 
	$(CXX) $(CXXFLAGS) Memory.cpp
core.o : 
	$(CXX) $(CXXFLAGS) core.cpp
main.o: 
	$(CXX) $(CXXFLAGS) main.cpp

run:
	./$(EXEC)
clean:
	$(RM) -rf $(OBJS) $(EXEC)