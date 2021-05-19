#include "helper.h"

int main(){    
    string fn;
    int Cores;
    int Simul;
    int rowaccess;
    int colaccess;
    int WaitBufferSize;
    cout<<"Please Enter the NUMBER_OF_CORES:";
    cin>>Cores;
    cout<<"Please Enter the Simulation_Cycles:";
    cin>>Simul;
    cout<<"Please Enter the ROW_ACCESS_DELAY:";
    cin>>rowaccess;
    cout<<"Please Enter the COL_ACCESS_DELAY:";
    cin>>colaccess;
    cout<<"Please Enter the WAIT_BUFFER_SIZE:";
    cin>>WaitBufferSize;
    vector<Core>Proc;
    int maxcomm=0;
    for(int i=0;i<Cores;i++){
        cout<<"Please Enter the Filename-"<<i+1<<":-"<<"\n";
        cin>>fn;
        Core c=Core(i+1,fn);
        maxcomm=max(c.totcomm,maxcomm);
        Proc.push_back(c);   
    }
    pair<int,int>access;
    access.first=rowaccess;
    access.second=colaccess;
    vector<int>rb(257,-1);
    int globbuff=-1;
    vector<vector<int>>Mem;
    for(int i=0;i<1024;i++){
        vector<int>M1;
        for(int j=0;j<256;j++){
            M1.push_back(0);
        }
        Mem.push_back(M1);   
    }
    set<int>addr;

    list<vector<int>>DRAM;
    int ctr=0;
    vector<string>to_print;
    vector<int>CheckExec;
    vector<int>nxtline(Cores,0);
    int i=-1;
    pair<string,int>PSI;
    int comtot=0;
  
    string dependent="#";
    int j=0;
    int cyc=0;                        //1 for T 2 for J 3 for LW/SW 4 for labels
    while(cyc<=Simul){            
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
                if(find(temp.begin(),temp.end(),dependent)==temp.end()){
                    PSI=executeTCommands(c,temp);
                    if(PSI.second==-1){
                        nxtline[j]=-1;                       
                    }
                    else{
                        nxtline[j]+=1;
                    }
                    comtot+=1;
                    cout<<"CORE "<<c.corenum<<":-"<<"Cycle No:-"<<cyc<<" "<<PSI.first<<"\n";   

                }
                                                    
                j+=1;
            }
            else if(ty==2){
                
                if(find(temp.begin(),temp.end(),dependent)==temp.end()){
                    PSI=executeJCommands(c,temp);
                    if(PSI.second==-1){
                        nxtline[j]=-1;
                    }
                    else{
                        if(PSI.second!=-2){
                             nxtline[j]=PSI.second;
                        }
                        else{
                            nxtline[j]+=1;
                        }
                       
                    }
                    comtot+=1;
                    cout<<"CORE "<<c.corenum<<":-"<<"Cycle No:-"<<cyc<<" "<<PSI.first<<"\n";
                }                  
                j+=1;                    
            }
            else if(ty==-1){
                comtot+=1;
                cout<<"CORE "<<c.corenum<<":-"<<"Unknown Command"<<"\n";
                nxtline[j]=-1;
                j+=1;
            }
            else if(ty==-3){
                comtot+=1;
                cout<<"CORE "<<c.corenum<<":-"<<"Cycle No:-"<<cyc<<" "<<"MAIN"<<"\n";
                nxtline[j]+=1;
                j+=1;
            }
            else if(ty==4){
                string h1=temp[0];
                string hz=h1.substr(0,h1.length()-1);
                comtot+=1;
                cout<<"CORE "<<c.corenum<<":-"<<"Cycle No:-"<<cyc<<" "<<"LABEL NAME"<<" "<<hz<<" "<<"\n";
                nxtline[j]+=1;
                j+=1;
            }
            else if(ty==-2){
                nxtline[j]=-1;
                j+=1;
                
            }
            else if(ty==3){
                if(DRAM.size()>=WaitBufferSize){
                    comtot+=1;
                    cout<<"CORE "<<c.corenum<<":-"<<"Cycle No:-"<<cyc<<" "<<"DRAM IS FULL"<<"\n"; 
                    j+=1;
                }
                else{  
                                       
                    PSI=executeMCommands(c,temp,Mem,globbuff,access,rb,addr);
                    int z=rb[256];
                    if(PSI.second==-1){
                        nxtline[j]=-1;
                        comtot+=1;
                        cout<<"CORE "<<c.corenum<<":-"<<"Cycle No:-"<<cyc<<" "<<PSI.first<<"\n";               
                        j+=1;
                    }
                    else{
                        if(temp[0]=="lw"){
                            dependent=temp[1];
                        }
                        else{
                            dependent="#";
                        }
                        comtot+=1;
                        cout<<"CORE "<<c.corenum<<":-"<<"Cycle No:-"<<cyc<<" "<<"DRAM-Request Issued"<<"\n"; 
                        nxtline[j]+=1;
                        j+=1;                           
                        vector<int>vec1;                         
                        vec1.push_back(c.corenum);
                        vec1.push_back(PSI.second+bime(z));
                        vec1.push_back(cyc);
                        vec1.push_back(ctr);
                        vec1.push_back(0);
                        vec1.push_back(0);
                        to_print.push_back(PSI.first);
                        DRAM.push_back(vec1);
                        CheckExec.push_back(0);
                        //wait=vec1[2]+vec1[1];
                        ctr+=1; 
                    }  
                    
                }
                
            }
            

        }
        if(j==Cores){
            if(DRAM.size()!=0){
                //int cycf=0;
                vector<int>&V11=DRAM.front();
                //corenumber timetoexecute cycleinwhichDRAMwasissued cycleinwhichitends correspondingPrint 
                if(cyc>V11[2] && V11[4]==0 && (cyc+V11[1]-1)<Simul){
                    cout<<"CORE "<<V11[0]<<":-"<<"Cycle No:-"<<cyc<<"-"<<cyc+V11[1]-1<<" "<<to_print[V11[3]]<<"\n"; 
                    CheckExec[V11[3]]=-1;
                    V11[4]=1; 
                    V11[5]=cyc+V11[1]-1;                           
                    
                }
                if(cyc==V11[5]){
                    DRAM.pop_front();
                    dependent="#";
                }
                
                    
            }                
            cyc+=1;
            bool b1=iscomplete(nxtline);
            bool b2=iscomplete(CheckExec);
            if(b1 && b2){
                break;
            }
            else{
                j=0;
            }
        }
    

}
    for(auto cr:Proc){
        cout<<"\n";
        printinst(cr);
        cout<<"\n";
        printregcontents(cr);        
    }
    cout<<"\n";
    printMemoryContents(Mem,addr);
    cout<<"\n";
    double thr=0.0;
    thr=(comtot+0.0)/(cyc+0.0);
    cout<<"Throughput-Value:-"<<" "<<thr<<"\n";

}
    



