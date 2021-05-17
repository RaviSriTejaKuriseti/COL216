//Register Set is Unique for each core
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <utility>
#include <string>
#include <list>
#include <iterator>
using namespace std;

class Core{
    public:
        int corenum;
        int totcomm;
        pair<int,string>fwcommand;  //firstwrongcommand line number and corresponding string
        vector<vector<string>>commands;   //File converted into commands
        vector<pair<string,int>>indices; //Indices for jump or branch labels
        map<int,int>indexmap; //Indices for jump or branch labels mapped to line number
        map<string,int>usedreg;     //USED REGISTERS
        map<string,int>unusedreg;      //UNUSED REGISTERS
        map<string,int>instrcounter;  //No of times each instruction is executed
        set<string>allreg;            //Set of all the registers
        Core(int n,string filename);
        vector<string>tokenize(const string str,const regex re);
        pair<int,string>fileread(string s,vector<pair<string,int>> &indices,vector<vector<string>> &filecontent); 
        void setNum(int n);
        void initIndMap();
        void readFile(string fname);
        void setRegs();
        void setInstruct();
              
};