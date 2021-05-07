#include "finitequeue.h"

 FixSizeQueue::FixSizeQueue(int l){
       initself();
       initlen(l);
    }

    
    void FixSizeQueue::initself(){
        deque<pair<int,int>>D;
        this->self=D;
    }

     void FixSizeQueue::initlen(int l){
        this->len=l;
    }

    

    bool FixSizeQueue::isEmpty(){
        return (self.size()==0);
    }

    bool FixSizeQueue::isFull(){
        return (len==self.size());
    }

    void FixSizeQueue::Pop(){
        self.pop_front();
    }

     void FixSizeQueue::Push(pair<int,int> &P){
        self.push_back(P);
    }

    pair<int,int> FixSizeQueue::Top(){
        auto it=self.begin();
        return *it;
    }
