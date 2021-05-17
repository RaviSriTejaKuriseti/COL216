#include "Core.h"

bool compstrings(string S1,string S2){
    if(S1[0]!='-' && S2[0]!='-'){
        if(S1.length()!=S2.length()){
            return S1.length()>S2.length();
        }
        else{
            int i=0;
            while(i<S1.length()){
                if(S1[i]==S2[i]){
                    i+=1;
                }
                else{
                    return S1[i]>S2[i];
                }
            }
                                    //true if a>=b false otherwise
        }

    }
    else if(S1[0]=='-' && S2[0]=='-'){
        if(S1.length()!=S2.length()){
            return S1.length()<S2.length();
        }
        else{
            int i=0;
            while(i<S1.length()){
                if(S1[i]==S2[i]){
                    i+=1;
                }
                else{
                    return S1[i]<S2[i];
                }

            }

        }

    }
    else{
        if(S1[0]=='-'){
            return false;
        }
        else{
            return true;
        }

    }
    return true;

}

bool checkvalidstring(string S){
    int i=0;
    if(S[0]=='-'){
        i+=1;
    }
    int j=i;
    while(j<S.length()){
        int check=S[j]-'0';
        if(check >=0 && check<=9){
            j+=1;
        }
        else{
            return false;
        }
    }
    return true;

}


bool checkinrange(string S,string low,string high){
    //returns true if low<=S<=high
    string X;
    int i=0;
    int start=0;
    if(S[0]=='-'){
        i+=1;
    }
    
    while(i<S.length() && S[i]=='0'){
        i+=1;
    }
    if(i==S.length()){
        start=i-1;
        X=S.substr(start,1);
        return true;
    }
    else{
        start=i;
        if(S[0]!='-'){
            X=S.substr(start,S.length()-start);
        } 
        else{
            X=S.substr(start,S.length()-1-start);
        }
        bool f1=compstrings(X,low);
        bool f2=compstrings(X,high);
        return (f1==true)&(f2==false);
    }
    return true;

}
bool intlimit(string S){
    string intmin="-2147483648";
    string intmax="2147483647";
    return checkinrange(S,intmin,intmax);   
}

bool icommandlimit(string S){
    string intmin="-32768";
    string intmax="32767";
    return checkinrange(S,intmin,intmax);
}


pair<string,bool> validatereg(Core &C,vector<string> &cmnd,int startindex,int endindex){
    map<string,int>M1=C.usedreg;
    set<string>S=C.allreg;
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

int jump(Core &C,string label){
    vector<pair<string,int>> I=C.indices;
    string t=label+":";
    for(int i=0;i<I.size();i++){
        if(I[i].first==t){
            return I[i].second;        //for the sake of jump-type instructions.
        }
    }
    return -1;
}

 




int cmndtyp(vector<string> &cmnd){
    set<string>Tcomm{"addi","add","sub","mul","slt"};
    set<string>Jcomm{"j","bne","beq"};
    set<string>Mcomm{"lw","sw"};
    bool b1=(Tcomm.find(cmnd[0])!=Tcomm.end());
    if(b1){
        return 1;    
    }
    else{
        bool b2=(Jcomm.find(cmnd[0])!=Jcomm.end());
        if(b2){
            return 2;
        }
        else{
            bool b3=(Mcomm.find(cmnd[0])!=Mcomm.end());
            if(b3){
                return 3;
            }
            else{
                if(cmnd.size()!=1){
                    return -1;
                }
                else{
                    if(cmnd[0]=="exit:"){
                        return -2;
                    }
                    else if(cmnd[0]=="main:"){
                        return -3;
                    }
                    else{
                        int x=(cmnd[0].length())-1;
                        if(cmnd.size()==1 && cmnd[0][x]==':'){
                            return 4;

                        }
                    return -1;
                    }
                }
               

            }

        }
       
    }
}

pair<string,int> executeMCommands(Core &C,vector<string> &cmnd,vector<vector<int>>&memory,int &globbuff,pair<int,int> &access,vector<int>&rowbuffer,set<int> &address){
    map<string,int>&Counter=C.instrcounter;                //T3 commands are lw,sw
    map<string,int>&usedreg=C.usedreg;  
    pair<string,int>P;
    P.first="NULL";
    P.second=-1;   //returns 0 if success -1 else

    if(cmnd.size()!=3){
        P.first="Incorrect Number Of Arguments for "+cmnd[0]+":-"+to_string(cmnd.size());
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
            P.first="Invalid Format of argument for "+cmnd[0]+":-"+s;
            return P;
        }
        left=j;
        string s1=s.substr(0,left);
        string r2=s.substr(left+1,l-2-left);
        
        bool b1=icommandlimit(s1);
        if(!b1){
          P.first="Not a 16-bit integer";
          return P;
        }
        vector<string>cmnda;
        cmnda.push_back(r1);
        cmnda.push_back(r2);
        pair<string,bool>VPB=validatereg(C,cmnda,0,1);
        if(VPB.second==false){
            P.first=VPB.first;
            return P;
        }
        if(r1=="$zero"){
            P.first="Cannot change value of Register $zero";
            return P;
        }
       
        
        int x=stoi(s1);
        int y=usedreg.at(r2);       
        int add=x+y;
        int r=add/1024;
        int col=add%256;
        int row=globbuff;
        int rowaccess=access.first;
        int colaccess=access.second;
        // vector<int>rowbuffer=rb;
        // vector<vector<int>>memory=M;
          
        if(cmnd[0]=="lw"){
            Counter["lw"]+=1;
            P.first=r1+" "+to_string(memory[r][col]);
                if(row==r){   
                    usedreg.at(r1)=memory[r][col];                
                    P.second=colaccess;
                    //upd+=0;
                    return P;
                    
                }
                else if(row==-1){
                    for(int D=0;D<256;D++){
                        rowbuffer[D]=memory[r][D];
                    }
                    rowbuffer[256]=r;
                    usedreg.at(r1)=memory[r][col];
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

                     usedreg.at(r1)=memory[r][col];
                     P.second=2*rowaccess+colaccess;
                     //upd+=2;
                     return P;                               
        }
    }
            else{
                Counter["sw"]+=1; 
                address.insert(add);       
                int x=usedreg.at(r1);
                P.first=to_string(add)+"-"+to_string(add+3)+":"+to_string(x);
               
                if(row==r){
                    memory[r][col]= usedreg.at(r1);
                    rowbuffer[col]=usedreg.at(r1);
                    P.second=colaccess; 
                    //upd+=1;                   
                    return P;
                   
                }
                else if(row==-1){
                    for(int D=0;D<256;D++){
                        rowbuffer[D]=memory[r][D];
                    }
                    rowbuffer[256]=r;
                    memory[r][col]= usedreg.at(r1);
                    rowbuffer[col]=usedreg.at(r1); 
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

                    memory[r][col]= usedreg.at(r1);
                    rowbuffer[col]=usedreg.at(r1); 
                    P.second=2*rowaccess+colaccess;
                    return P;
                   

                   
                }
            
            }
            
   }   
}

 

pair<string,int> executeTCommands(Core &C,vector<string> &cmnd){
    map<string,int>&instrcounter=C.instrcounter;                //T commands are addi,add,sub,mul,slt
    map<string,int>&usedreg=C.usedreg;
    pair<string,int>P;
    P.first="NULL";
    P.second=-1;                         //returns 1 for successful -1 else
    if(cmnd[0]=="addi"){
        if(cmnd.size()!=4){
            P.first="Incorrect Number Of Arguments for addi:-"+to_string(cmnd.size());
            return P;
        }
        else{
            instrcounter["addi"]+=1;
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
            pair<string,bool> VPB=validatereg(C,cmnd,1,2);
            if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }
            
            usedreg.at(r1)= usedreg.at(r2)+stoi(r3);            
            P.first=r1+":"+to_string(usedreg.at(r1));
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
            instrcounter["add"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            string r3=cmnd[3];
            pair<string,bool> VPB=validatereg(C,cmnd,1,3);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }    
            usedreg.at(r1)= usedreg.at(r2)+usedreg.at(r3);            
            P.first=r1+":"+to_string(usedreg.at(r1));
            return P;  
        }
    }
    else if(cmnd[0]=="sub"){
        if(cmnd.size()!=4){
           P.first="Incorrect Number Of Arguments for sub:-"+to_string(cmnd.size());
           return P;
        }
        else{
            instrcounter["sub"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            string r3=cmnd[3];
            pair<string,bool> VPB=validatereg(C,cmnd,1,3);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }
           
            usedreg.at(r1)=usedreg.at(r2)-usedreg.at(r3);            
            P.first=r1+":"+to_string(usedreg.at(r1));
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
            instrcounter["mul"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            string r3=cmnd[3];
            pair<string,bool> VPB=validatereg(C,cmnd,1,3);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }
            usedreg.at(r1)= usedreg.at(r2)*usedreg.at(r3);
            P.first=r1+":"+to_string(usedreg.at(r1));
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
            instrcounter["slt"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            string r3=cmnd[3];
            pair<string,bool> VPB=validatereg(C,cmnd,1,3);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }
            if(r1=="$zero"){
                P.first="Cannot change value of Register $zero";
                return P;

            }
            if(usedreg.at(r2)<usedreg.at(r3)){
                usedreg.at(r1)=1;                
                P.first=r1+":"+to_string(usedreg.at(r1));
                P.second=1;
                return P;
            }
            else{
                usedreg.at(r1)=0;                
                P.first=r1+":"+to_string(usedreg.at(r1));
                P.second=1;
                return P;

            }  
                 
        }        
           
        }
        return P;
}


pair<string,int> executeJCommands(Core &C,vector<string> &cmnd){
    map<string,int>&instrcounter=C.instrcounter;                //T2 commands are beq,bne,j
    map<string,int>&usedreg=C.usedreg;                         //returns label-line number for successful -2 for no branch else -1
    vector<pair<string,int>> &I=C.indices;
    pair<string,int>P;
    P.first="NULL";
    P.second=-1;    
    if(cmnd[0]=="j"){
        if(cmnd.size()!=2){
            P.first="Incorrect Number Of Arguments for j:-"+to_string(cmnd.size());
            return P;
        }
        else{
             instrcounter["j"]+=1;
             int l=jump(C,cmnd[1]);
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
            instrcounter["beq"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            pair<string,bool> VPB=validatereg(C,cmnd,1,2);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }      

            if(usedreg.at(r1)==usedreg.at(r2)){                    
                int l=jump(C,cmnd[3]);
                if(l==-1){
                    cout<<"Branch "<<cmnd[3]<<" not found"<<"\n";
                    return P;
                }                
                P.first="Branching to Label:-"+cmnd[3];
                P.second=l;
                return P;
                }
                else{
                P.first="No-Branching to Label:-"+cmnd[3];
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
            instrcounter["bne"]+=1;
            string r1=cmnd[1];
            string r2=cmnd[2];
            pair<string,bool> VPB=validatereg(C,cmnd,1,2);
           if(VPB.second==false){
                P.first=VPB.first;
                return P;
            }      

            if(usedreg.at(r1)!=usedreg.at(r2)){                    
                int l=jump(C,cmnd[3]);
                if(l==-1){
                    cout<<"Branch "<<cmnd[3]<<" not found"<<"\n";
                    return P;
                }
                P.first="Branching to Label:-"+cmnd[3];
                P.second=l;
                return P;
                }
                else{
                P.first="No-Branching to Label:-"+cmnd[3];
                P.second=-2;
                return P;
                }

            }
            

        }
        return P;
}



bool iscomplete(vector<int> &vec){
    int m=-1;
    for(int i=0;i<vec.size();i++){
        m=max(m,vec[i]);
    }
    return (m==-1);
}

void printregcontents(Core &C){
    map<string,int>M1=C.usedreg;
    map<string,int>M2=C.unusedreg;
    for(auto x:C.allreg){
        if(M1.find(x)!=M1.end()){
            cout<<x<<":" <<hex<<M1.at(x)<<" ";
        }
        else{
             cout<<x<<":" <<hex<<M2.at(x)<<" ";
        }
    }
    cout<<"\n";
}


void printinst(Core &C){
    map<string,int>M3=C.instrcounter;
    cout<<"CORE:-"<<C.corenum<<"\n";
    int cpi=0;
    for(auto it=M3.begin();it!=M3.end();it++){
        cout<<it->first<<" "<<"executed"<<" "<<it->second<<" "<<"times"<<"\n";
        cpi+=it->second;
    }
  
}

void printMemoryContents(vector<vector<int>>&memory,set<int>&s){
    int r=0;int c=0;int add=0;
    cout<<"MEMORY CONTENTS:-"<<"\n";
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

int bise(int x,int start,int end,int ct){
    int mid=(start+end)/2;
    if(x==mid){
        return ct;
    }
    else if(x==start){
        return start;
    }
    else if(x==end){
        return end;
    }
    else if(x<mid){
        return bise(x,start,mid-1,ct+1);
    }
    else{
        return bise(x,mid,end,ct+1);
    }

}


int bime(int x){
    return bise(x,0,1023,1);
}
  