#include "core.h"
#include "Memory.h"

// void executeCommands(vector<Core> &vec, Memory *M){
//     set<string>Tcomm{"addi","add","sub","mul","slt"};
//     set<string>Jcomm{"j","bne","beq"};
//     set<string>Mcomm{"lw","sw"};
//     for(int j=0;j<vec.size();i++){
//         Core c=vec[j];
//         int i=0;
//         while(i<c.commands.size()){
//         vector<string>cmnd=c.commands[i];
//         bool b1=(Tcomm.find(cmnd[0])!=Tcomm.end());
//         if(b1){
//             int n=c.executeTCommands(cmnd);
//             if(n==1){
//                 i+=1;
//             }
//             else{
//                 return;
//             }
           
//         }
//         else{
//             bool b2=(Jcomm.find(cmnd[0])!=Jcomm.end());
//             if(b2){
//                 int n=c.executeJCommands(cmnd);
//                 if(n==-1){
//                     return;
//                 }
//                 else if(n==-2){
//                     i+=1;
//                 }
//                 else{
//                     i=n;
//                 }
//             }
//             else{
//                 bool b3=(Mcomm.find(cmnd[0])!=Mcomm.end());
//                 if(b3){
//                     //int n=executeLCommands(cmnd);
//                     ;

//                 }
//                 else{
//                     int x=cmnd[0].length()-1;
//                     if(cmnd.length()==1 && cmnd[0][x]==':'){
//                         i+=1;
//                     }
//                     else{
//                         return;
//                     }

//                 }

//             }

//         }
      
        
       
//     }
    

//     }
    

// }


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
    for(int i=0;i<Cores;i++){
        cout<<"Please Enter the Filename:";
        cin>>fn;
        Core c=Core(i+1,fn);
        Proc.push_back(c); 
        cout<<c.corenum<<"\n";      
    }
    Memory M=Memory(Cores,rowaccess,colaccess);
    


}