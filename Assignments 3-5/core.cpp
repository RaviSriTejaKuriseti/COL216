#include "core.h"

#define oper remove_if(tokenized.begin(), tokenized.end(),[](string const& s) { return s.size() == 0;})


Core::Core(int n,string fname){
    setNum(n);
    readFile(fname);
    setRegs();
    setInstruct();
    initIndMap();
}

void Core::setNum(int n){
    this->corenum=n;
}
 
void Core::readFile(string s){
    vector<pair<string,int>>indices;
    vector<vector<string>>filecontent;
    pair<int,string>P1=fileread(s,indices,filecontent);
    this->indices=indices;
    this->commands=filecontent;
    this->fwcommand=P1;
    this->totcomm=filecontent.size();
}

void Core::setInstruct(){
    map<string,int>instrcounter;
    instrcounter.insert({"addi",0}); instrcounter.insert({"add",0});instrcounter.insert({"sub",0}); 
    instrcounter.insert({"mul",0}); instrcounter.insert({"slt",0});instrcounter.insert({"lw",0});
    instrcounter.insert({"sw",0}); instrcounter.insert({"beq",0}); instrcounter.insert({"bne",0});
    instrcounter.insert({"j",0});
    this->instrcounter=instrcounter;

}

void Core::setRegs(){  
    map<string,int>usereg;
    map<string,int>unusereg;
    set<string>areg; 
    vector<string>com={"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};
    for(auto x:com){
        areg.insert(x);
    }
    int A=0;
    while(A<32){
        if(com[A]=="$sp"){
            unusereg.insert({com[A],2147479548});

        }
        else if(A==1 || A>25){
            unusereg.insert({com[A],0});
        }
        
        else{
            usereg.insert({com[A],0});

        }
        A++;
    }
    this->allreg=areg;
    this->usedreg=usereg;
    this->unusedreg=unusereg;

}

vector<string>Core::tokenize(const string str,const regex re){
     int c=count(str.begin(),str.end(),',');    
	sregex_token_iterator it{ str.begin(), 
							str.end(), re, -1 }; 
	vector<string> tokenized{ it, {} }; 

	
	tokenized.erase(oper,tokenized.end()); 
    if(c==tokenized.size()-2 || (tokenized.size()==1 && c==0)){
        tokenized.push_back("VALID");
        tokenized.push_back("true");
    }
    else{
        tokenized.push_back(str);
        tokenized.push_back("false");
        
    }

	return tokenized; 
}

pair<int,string> Core::fileread(string s,vector<pair<string,int>>&indices,vector<vector<string>> &filecontent){    
    const regex re(R"([\s|,]+)");    
    ifstream in(s);
    string line;
    int count=0;
    int c1=-1;
    string ans="NULL";
    while(getline(in,line)){
        vector<string> temp=tokenize(line,re);
        string check=temp.back();
        temp.pop_back();
        string s1=temp.back();
        temp.pop_back();
        if(temp.size()!=0){
             filecontent.push_back(temp);
        }
        else{
            count-=1;
        }
        if(check=="false" && temp.size()>0 && c1==-1){
            c1=count; 
            ans=s1;          
        }
           
        if(temp.size()==1){
            pair<string,int>p;
            p.first=temp[0];
            p.second=count;
            indices.push_back(p);
        }
        count+=1;

    }
    in.close();
    pair<int,string>P;
    P.first=c1;
    P.second=ans;
    sort(indices.begin(),indices.end(),[](pair<string,int>a,pair<string,int>b)
{ 
    return a.second<b.second; 
});;

    return P;

}

void Core::initIndMap(){
    vector<pair<string,int>>PS=this->indices;
    map<int,int>M;
    for(int i=0;i<PS.size();i++){
        pair<int,int>P;
        P.first=PS[i].second;
        P.second=i;
        M.insert(P);
    }
    this->indexmap=M;
}













