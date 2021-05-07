#include <iostream>
#include <deque>

using namespace std;



class FixSizeQueue{
    public:
    int len;
    deque<pair<int,int>>self;
    void initself();
    void initlen(int l);
    void Push(pair<int,int> &P);
    void Pop();
    pair<int,int>Top();
    FixSizeQueue(int size);
    bool isEmpty();
    bool isFull(); 

};