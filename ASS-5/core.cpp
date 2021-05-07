#include "core.h"
#include "auxiliary.h"
#include "Memory.h"

#define oper remove_if(tokenized.begin(), tokenized.end(),[](string const& s) { return s.size() == 0;})

Core::Core(int n,string fname){
    setNum(n);
    readFile(fname);
    setRegs();
    setInstruct();
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
    map<string,int>usedreg;
    map<string,int>unusedreg;
    set<string>allreg; 
    vector<string>com={"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};
    for(auto x:com){
        allreg.insert(x);
    }
    int A=0;
    while(A<32){
        if(com[A]=="$sp"){
            unusedreg.insert({com[A],2147479548});

        }
        else if(A==1 || A>25){
            unusedreg.insert({com[A],0});
        }
        
        else{
            usedreg.insert({com[A],0});

        }
        A++;
    }
    this->allreg=allreg;
    this->usedreg=usedreg;
    this->unusedreg=unusedreg;

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

    return P;

}

void Core::printregcontents(){
    map<string,int>M1=this->usedreg;
    map<string,int>M2=this->unusedreg;
    for(auto x:this->allreg){
        if(M1.find(x)!=M1.end()){
            cout<<x<<":" <<hex<<M1.at(x)<<" ";
        }
        else{
             cout<<x<<":" <<hex<<M2.at(x)<<" ";
        }
    }
    cout<<"\n";
}


void Core::printinst(){
    map<string,int>M3=this->instrcounter;
    int cpi=0;
    for(auto it=M3.begin();it!=M3.end();it++){
        cout<<it->first<<" "<<"executed"<<" "<<it->second<<" "<<"times"<<"\n";
        cpi+=it->second;
    }
  
}

pair<string,bool> Core::validatereg(vector<string> &cmnd,int startindex,int endindex){
    map<string,int>M1=this->usedreg;
    set<string>S=this->allreg;
    bool b1;
    bool b2;
    pair<string,bool>PB;
    PB.first="NIL";
    PB.second=false;
    for(int i=startindex;i<=endindex;i++){
        b1=(S.find(cmnd[i])!=S.end());
        if(b1==false){
            PB.first="Invalid Register:-"+cmnd[i];
            return PB;
        }
        b2=(M1.find(cmnd[i])!=M1.end());
        if(b2==false){
            PB.first="Reserved Register NOT FOR USE:-"+cmnd[i];
            return PB;
        }               
    }
    PB.second=true;
    return PB;
}

int Core::jump(string label){
    vector<pair<string,int>> I=this->indices;
    string t=label+":";
    for(int i=0;i<I.size();i++){
        if(I[i].first==t){
            return I[i].second;        //for the sake of jump-type instructions.
        }
    }
    return -1;
}




pair<string,int> Core::executeTCommands(vector<string> &cmnd){
    map<string,int>Counter=this->instrcounter;                //T commands are addi,add,sub,mul,slt
    map<string,int>reg=this->usedreg;
    pair<string,int>P;
    P.first="NULL";
    P.second=-1;                         //returns 1 for successful -1 else
    if(cmnd[0]=="addi"){
        if(cmnd.size()!=4){
            P.first="Incorrect Number Of Arguments for addi:-"+to_string(cmnd.size());
            return P;
        }
        else{
            Counter["addi"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            string r3=cmnd[3];
            bool b1=checkvalidstring(r3);
            if(b1==false){
                P.first="Not an Integer:-"+r3;
                return P;
            }
            else{
                bool b2=icommandlimit(r3);
                if(b2==false){
                    P.first="Not a 16-bit Integer:-"+r3;
                    return P;
                }
                
            }
            pair<string,bool> VPB=validatereg(cmnd,1,2);
            if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }
            
            reg.at(r1)= reg.at(r2)+stoi(r3);            
            P.first=r1+":"+to_string(reg.at(r1));
            P.second=1;
            return P;
        }
       

    }
    else if(cmnd[0]=="add"){
        if(cmnd.size()!=4){
           P.first="Incorrect Number Of Arguments for add:-"+to_string(cmnd.size());
           return P;
        }
        else{
            Counter["add"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            string r3=cmnd[3];
            pair<string,bool> VPB=validatereg(cmnd,1,3);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }    
            reg.at(r1)= reg.at(r2)+reg.at(r3);            
            P.first=r1+":"+to_string(reg.at(r1));
            return P;  
        }
    }
    else if(cmnd[0]=="sub"){
        if(cmnd.size()!=4){
           P.first="Incorrect Number Of Arguments for sub:-"+to_string(cmnd.size());
           return P;
        }
        else{
            Counter["sub"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            string r3=cmnd[3];
            pair<string,bool> VPB=validatereg(cmnd,1,3);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }
           
            reg.at(r1)=reg.at(r2)-reg.at(r3);            
            P.first=r1+":"+to_string(reg.at(r1));
            P.second=1;
            return P;
                    
           
           
        }

    }
    else if(cmnd[0]=="mul"){
        if(cmnd.size()!=4){
           P.first="Incorrect Number Of Arguments for mul:-"+to_string(cmnd.size());
           return P;
        }
        else{
            Counter["mul"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            string r3=cmnd[3];
            pair<string,bool> VPB=validatereg(cmnd,1,3);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }
            reg.at(r1)= reg.at(r2)*reg.at(r3);
            P.first=r1+":"+to_string(reg.at(r1));
            P.second=1;
            return P;
           
        }


    }
    else if(cmnd[0]=="slt"){
        if(cmnd.size()!=4){
           P.first="Incorrect Number Of Arguments for slt:-"+to_string(cmnd.size());
           return P;
        }
        else{
            Counter["slt"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            string r3=cmnd[3];
            pair<string,bool> VPB=validatereg(cmnd,1,3);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }
            if(reg.at(r2)<reg.at(r3)){
                reg.at(r1)=1;                
                P.first=r1+":"+to_string(reg.at(r1));
                P.second=1;
                return P;
            }
            else{
                reg.at(r1)=0;                
                P.first=r1+":"+to_string(reg.at(r1));
                P.second=1;
                return P;

            }  
                 
        }        
           
        }
        return P;
}


pair<string,int> Core::executeJCommands(vector<string> &cmnd){
    map<string,int>Counter=this->instrcounter;                //T2 commands are beq,bne,j
    map<string,int>reg=this->usedreg;                         //returns label-line number for successful -2 for no branch else -1
    vector<pair<string,int>> I=this->indices;
    pair<string,int>P;
    P.first="NULL";
    P.second=-1;    
    if(cmnd[0]=="j"){

        if(cmnd.size()!=2){
            P.first="Incorrect Number Of Arguments for j:-"+to_string(cmnd.size());
            return P;
        }
        else{
             Counter["j"]+=1;
             int l=jump(cmnd[1]);
             if(l==-1){
                cout<<"Label "<<cmnd[1]<<" not found"<<"\n";
                return P;
             }
            
            P.first=" Jumping to Label:-"+cmnd[1];
            P.second=l;
            return P;            
        }

    }
    else if(cmnd[0]=="beq"){
        if(cmnd.size()!=4){
            cout<<"Incorrect Number Of Arguments for "<<cmnd[0]<<":-"<<cmnd.size()<<"\n";
            return P;
        }
        else{
            Counter["beq"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            pair<string,bool> VPB=validatereg(cmnd,1,2);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }      

            if(reg.at(r1)==reg.at(r2)){                    
                int l=jump(cmnd[3]);
                if(l==-1){
                    cout<<"Branch "<<cmnd[3]<<" not found"<<"\n";
                    return P;
                }                
                P.first="Branching to Label:-"+cmnd[1];
                P.second=l;
                return P;
                }
                else{
                P.first="No-Branching to Label:-"+cmnd[1];
                P.second=-2;
                return P;
                }

            }
           

    }
    else if(cmnd[0]=="bne"){
        if(cmnd.size()!=4){
            cout<<"Incorrect Number Of Arguments for "<<cmnd[0]<<":-"<<cmnd.size()<<"\n";
            return P;
        }
        else{
            Counter["bne"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            pair<string,bool> VPB=validatereg(cmnd,1,2);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }      

            if(reg.at(r1)!=reg.at(r2)){                    
                int l=jump(cmnd[3]);
                if(l==-1){
                    cout<<"Branch "<<cmnd[3]<<" not found"<<"\n";
                    return P;
                }
                P.first="Branching to Label:-"+cmnd[1];
                P.second=l;
                return P;
                }
                else{
                P.first="No-Branching to Label:-"+cmnd[1];
                P.second=-2;
                return P;
                }

            }
            

        }
        return P;
}

pair<string,int> executeMcommands(Core *C,vector<string> &cmnd,Memory *M){
    map<string,int>Counter=C->instrcounter;                //T3 commands are lw,sw
    map<string,int>reg=C->usedreg;  
    pair<string,int>P;
    P.first="NULL";
    P.second=-1;                        //returns 0 if success -1 else

    if(cmnd.size()!=3){
        cout<<"Incorrect Number Of Arguments for "<<cmnd[0]<<":-"<<cmnd.size()<<"\n";
        return P;
    }
    else{            
        string r1=cmnd[1];
        string s=cmnd[2];
        int j=0;
        int left=0;
        int l=s.length();
        while(j<l && s[j]!='('){
            j+=1;
        }
        if(j==l){
            cout<<"Invalid Format of argument for "<<cmnd[0]<<":-"<<s<<"\n";
            return P;
        }
        left=j;
        string s1=s.substr(0,left);
        string r2=s.substr(left+1,l-2-left);
        bool b1=icommandlimit(s1);
        if(!b1){
           cout<<"Not a 16-bit integer"<<"\n";
           return P;
        }
        vector<string>cmnda{r1,r2};
        pair<string,bool> VPB=C->validatereg(cmnda,1,2);
        if(VPB.second==false){
            P.first=VPB.first;
            return P;
        }
        
        int x=stoi(s1);
        int y=reg.at(r2);
        int add=x+y;
        int r=add/1024;
        int col=add%256;
        int row=M->getRBuffer();
        int rowaccess=M->getRowaccess();
        int colaccess=M->getColaccess();
        vector<int>rowbuffer=M->rbuff;
        vector<vector<int>>memory=M->memory;
          
        if(cmnd[0]=="lw"){
            Counter["lw"]+=1;
            P.first=r1+" "+to_string(memory[r][col]);
                if(row==r){   
                    reg.at(r1)=memory[r][col];                
                    P.second=colaccess;
                    //upd+=0;
                    return P;
                    
                }
                else if(row==-1){
                    for(int D=0;D<256;D++){
                        rowbuffer[D]=memory[r][D];
                    }
                    rowbuffer[256]=r;
                    reg.at(r1)=memory[r][col];
                    P.second=rowaccess+colaccess;
                    //upd+=1;
                    return P;

                }
                else{
                     for(int D=0;D<256;D++){
                        memory[r][D]=rowbuffer[D];
                    }
                   
                    int row=r;
                    for(int D=0;D<256;D++){
                        rowbuffer[D]=memory[r][D];
                    }
                     rowbuffer[256]=r;

                     reg.at(r1)=memory[r][col];
                     P.second=2*rowaccess+colaccess;
                     //upd+=2;
                     return P;                               
        }
    }
    else{
           Counter["sw"]+=1;
        
                int x=reg.at(r1);
                P.first=to_string(add)+"-"+to_string(add+3)+":"+to_string(x);
               
                if(row==r){
                    memory[r][col]= reg.at(r1);
                    rowbuffer[col]=reg.at(r1);
                    P.second=colaccess; 
                    //upd+=1;                   
                    return P;
                   
                }
                else if(row==-1){
                    for(int D=0;D<256;D++){
                        rowbuffer[D]=memory[r][D];
                    }
                    rowbuffer[256]=r;
                    memory[r][col]= reg.at(r1);
                    rowbuffer[col]=reg.at(r1); 
                    P.second=rowaccess+colaccess;
                    //upd+=2;
                    return P;
                    


                }
                else{
                     for(int D=0;D<256;D++){
                        memory[r][D]=rowbuffer[D];
                    }
                   
                    int row=r;
                    for(int D=0;D<256;D++){
                        rowbuffer[D]=memory[r][D];
                    }
                     rowbuffer[256]=r;

                    memory[r][col]= reg.at(r1);
                    rowbuffer[col]=reg.at(r1); 
                    P.second=2*rowaccess+colaccess;
                    return P;
                   

                   
                }
            
            }
            
   }   
}


