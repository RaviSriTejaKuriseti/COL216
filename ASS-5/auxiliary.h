#include <iostream>
#include <string>
using namespace std;

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

int commandtype (vector<string> &cmnd){
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
                int x=(cmnd[0].length())-1;
                if(cmnd.size()==1 && cmnd[0][x]==':'){
                    return 4;

                }
                else{
                    return -1;
                }

            }

        }
       
    }
}
    


