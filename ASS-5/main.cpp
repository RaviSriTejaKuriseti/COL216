#include "core.h"
#include "Memory.h"
#include "auxiliary.h"


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
    int maxcomm=0;
    for(int i=0;i<Cores;i++){
        cout<<"Please Enter the Filename:";
        cin>>fn;
        Core c=Core(i+1,fn);
        maxcomm=max(c.totcomm,maxcomm);
        Proc.push_back(c); 
        cout<<c.corenum<<"\n";      
    }
    Memory M=Memory(10,rowaccess,colaccess); //DRAM-SIZE=10
    vector<bool>check(true,Cores);
    for(int i=1;i<maxcomm-1;i++){
        int j=0;                        //1 for T 2 for J 3 for LW/SW 4 for labels
        while(j<Cores){
            if(check[j]=false){
                j+=1;
            }
            else{
                 Core c=Proc[j];
                 if(i<c.totcomm-1){
                    vector<string>temp=c.commands[i];
                    int ty=commandtype(temp);
                    if(ty==1){
                        c.executeTCommands(temp);
                    }
                    else if(ty==2){
                        c.executeJCommands(temp);
                    }
                    else if(ty==-1){
                        check[j]=false;
                    }
                    else if(ty==4){
                        continue;
                    }
                    else{
                        //c.


                    }
            
               
                
                }
                else{
                    j+=1;
                }
           

        }
    


    }

}