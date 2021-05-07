#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <string>
#include <list>
#include <iterator>
#include "finitequeue.h"
using namespace std;

class Memory{
    public:
    vector<vector<int>>memory;
    vector<int>rbuff;
    int globbuff=-1; //-1 because unintialized
    set<int>address;
    FixSizeQueue DRAM(int qsize);
    int rowaccess;
    int colaccess;
    Memory(int qsize,int rowaccess,int colaccess);
    void initMemory(int rowaccess,int colaccess);
    void initBuffer();
    void setRBuffer(int x);
    void initDRAM(int qsize);
    void initSet();
    int getRBuffer();
    void printMemoryContents();
    int getRowaccess();
    int getColaccess();

};