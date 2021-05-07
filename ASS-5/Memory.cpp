#include "Memory.h"


Memory::Memory(int qsize,int rowaccess,int colaccess){
    initMemory(rowaccess,colaccess);
    initBuffer();
    initDRAM(qsize);
    initSet();
}

void Memory::initMemory(int r,int c){
    vector<vector<int>> mem(1024, vector<int> (256, 0));
    this->memory=mem;
    this->rowaccess=r;
    this->colaccess=c;
}

void Memory::initBuffer(){
    vector<int>vec(257,0);
    vec[256]=-1;
    this->rbuff=vec;
}

void Memory::setRBuffer(int x){
    this->globbuff=x;
}

void Memory::initDRAM(int qsize){
    FixSizeQueue Q(qsize);
}

void Memory::initSet(){
    set<int>asd;
    this->address=asd;
}

void Memory::printMemoryContents(){
    set<int>s=this->address;
    vector<vector<int>>memory=this->memory;
    int r=0;int c=0;int add=0;
    if(s.size()==0){
        cout<<"NIL"<<"\n";
        return;
    }
    for(auto it=s.begin();it!=s.end();it++){
        add=*it;
        r=add/1024;
        c=add%256;
        cout<<to_string(add)<<"-"<<to_string(add+3)<<":"<<to_string(memory[r][c])<<"\n";

    }
}

int Memory::getRBuffer(){
    return this->globbuff;
}

int Memory::getRowaccess(){
    return this->rowaccess;
}

int Memory::getColaccess(){
    return this->colaccess;
}