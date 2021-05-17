CXX=g++
CXXFLAGS=-g -Wall -Wextra -c
#LINKER_FLAGS=
EXEC=a
OBJS=core.o main.o 

RM=rm

all:begin
default:begin

begin:core.o main.o 
	$(CXX) -o $(EXEC) $(OBJS)

core.o : 
	$(CXX) $(CXXFLAGS) core.cpp
main.o: 
	$(CXX) $(CXXFLAGS) main.cpp

run:
	./$(EXEC)
clean:
	$(RM) -rf $(OBJS) $(EXEC)