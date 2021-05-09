#include <iostream>
#include <deque>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

using namespace std;



class FixSizeQueue{
    public:
    int len;
    deque<vector<string>>self;
    void initself();
    void initlen(int l);
    void Push(vector<string> &P);
    void Pop();
    vector<string>Top();
    FixSizeQueue(int size);
    bool isEmpty();
    bool isFull(); 

};