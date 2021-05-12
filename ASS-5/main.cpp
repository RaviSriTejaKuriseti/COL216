#include "helper.h"

int main(){    
    string fn;
    int Cores;
    int Simul;
    int rowaccess;
    int colaccess;
    cout<<"Please Enter the NUMBER_OF_CORES:";
    cin>>Cores;
    cout<<"Please Enter the Simulation_Cycles:";
    cin>>Simul;
    cout<<"Please Enter the ROW_ACCESS_DELAY:";
    cin>>rowaccess;
    cout<<"Please Enter the COL_ACCESS_DELAY:";
    cin>>colaccess;
    vector<Core>Proc;
    vector<FixSizeQueue>Cache;
    int maxcomm=0;
    for(int i=0;i<Cores;i++){
        cout<<"Please Enter the Filename:";
        cin>>fn;
        //Core *c=new Core(i+1,fn);
        Core c=Core(i+1,fn);
        FixSizeQueue f=FixSizeQueue(2);
        maxcomm=max(c.totcomm,maxcomm);
        Proc.push_back(c); 
        Cache.push_back(f);
        //cout<<c->corenum<<"\n";      
    }
    Memory M=Memory(10,rowaccess,colaccess); //DRAM-SIZE=10
    //vector<bool>check(Cores,true);
    vector<int>nxtline(Cores,1);
    int i=-1;
    pair<string,int>PSI;
        int j=0;                        //1 for T 2 for J 3 for LW/SW 4 for labels
        while(true){
            
            if(j==Cores){
                bool b1=iscomplete(nxtline);
                if(b1){
                    break;
                }
                else{
                    j=0;
                }
            }
            if(nxtline[j]==-1){
                j+=1;
            }
            else{
                Core &c=Proc[j];
                i=nxtline[j];
                vector<vector<string>>TT=c.commands;
                vector<string>temp=TT[i];
                int ty=cmndtyp(temp);
                if(ty==1){
                    pair<string,int>PSI=executeTCommands(c,temp);
                    nxtline[j]+=1;
                    j+=1;
                    //checklines[i][j]=true;
                }
                else if(ty==2){
                    PSI=executeJCommands(c,temp);
                    nxtline[j]=PSI.second;
                    j+=1;
                    //checklines[i][j]=true;
                    // map<int,int>M=c.indexmap;
                    // int v1=PSI.second;
                    // int v2=M[v1];
                    // string lbl=(c.indices[v2]).first;
                    // if(lbl=="exit:"){
                    //     nxtline[j]=-1;
                    // }
                    // else{
                    //     int u=v2+1;
                    //     int u1=M[u];
                        // for(int V=v1;V<u1;V++){
                        //     checklines[V][j]=false;
                        // }

                    //}
                    
                }
                else if(ty==-1){
                    nxtline[j]=-1;
                    j+=1;
                }
                else if(ty==4){
                    nxtline[j]+=1;
                    j+=1;
                    //checklines[i][j]=true;
                }
                else if(ty==-2){
                    printinst(c);
                    printregcontents(c);
                    nxtline[j]=-1;
                    j+=1;
                    
                }
                else if(ty==3){
                    FixSizeQueue f=Cache[j];
                    f.Push(temp);
                    int k=nxtline[j]+1;
                    while(k<c.totcomm){
                        vector<string>temp1=(c.commands)[k];
                        int ty2=cmndtyp(temp1);
                        if(ty2==1){
                            PSI=executeTCommands(c,temp1);
                            k+=1;
                        }
                        else if(ty2==2){
                            PSI=executeJCommands(c,temp);
                            k=PSI.second;
                        }
                        else if(ty2==4){
                            k+=1;
                        }
                        else{
                            break;
                        }
                    }
                    nxtline[j]=k;
                    j+=1;
                }
                

                }              
    
        }
    


    }

