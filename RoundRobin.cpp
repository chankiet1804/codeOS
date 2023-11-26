#include <iostream>
#include <math.h>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;
struct Process{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} typedef PCB;

void inputProcess(int n, vector<PCB> &P)
{
    
    for(int i=0;i<n;i++)
    {
        cout<<"Enter the Process Name,Arrival Time & Burst Time: ";
        int x,y,z;cin>>x>>y>>z;
        PCB temp;
        temp.iPID=x;
        temp.iArrival=y;
        temp.iBurst=z;
        temp.iStart=temp.iFinish=temp.iResponse=temp.iWaiting=temp.iTaT=0;
        P.push_back(temp);
    }
   
}
void outputProcess(int n,vector<PCB> P)
{
    cout<<"RoundRobin Algorithm:\n";
    cout<<"Process\tArrival\tBurst\tStart\tFinish\tWaiting\tResponse\tTurnAround\n";
    for(int i=0;i<n;i++)
    {
        cout<<P[i].iPID<<"\t"<<P[i].iArrival<<"\t"<<P[i].iBurst<<"\t"<<P[i].iStart<<"\t"<<P[i].iFinish<<"\t"<<P[i].iWaiting<<"\t"<<P[i].iResponse<<"\t"<<P[i].iTaT<<"\n";
    }
}

//hàm sắp xếp process theo thứ tự tăng dần arrival time
bool cmp (PCB a, PCB b)
{
    return a.iArrival<b.iArrival;
}

//hàm sắp xếp process theo thứ tự tăng dần PID
bool cmp2 (PCB a,PCB b)
{
    return a.iPID<b.iPID;
}

void execute(int n,int q,vector<PCB> Input,vector<PCB> &Terminated,int &iReady, int &iTerminated)
{
    queue<PCB> que;
    int curTime=0;
    int remain_burst[100]; // mang luu giu gia tri burst time cua cac process
    for(int i=0;i<n;i++)
    {
        remain_burst[Input[i].iPID]=Input[i].iBurst;
    }
    while(iTerminated<n)
    {
        while (iReady < n && Input[iReady].iArrival <= curTime) // đưa các process đã đến vào hàng đợi
        {
            que.push(Input[iReady]);
            iReady++;
        }
        if(!que.empty()) // neu co tien trinh trong hang doi
        {
            PCB currentProcess = que.front();
            que.pop();
            if(currentProcess.iBurst<=q and currentProcess.iBurst==remain_burst[currentProcess.iPID]) // tiến trình lần đầu được thực thi
            {
                currentProcess.iStart=curTime;
                currentProcess.iFinish=currentProcess.iStart+currentProcess.iBurst;
                currentProcess.iResponse=currentProcess.iStart-currentProcess.iArrival;
                currentProcess.iTaT=currentProcess.iFinish-currentProcess.iArrival;
                currentProcess.iWaiting=currentProcess.iTaT-currentProcess.iBurst;
                curTime+=currentProcess.iBurst;
                Terminated.push_back(currentProcess);
                iTerminated++;               
            }
            else if(currentProcess.iBurst>q and currentProcess.iBurst==remain_burst[currentProcess.iPID]) //tiến trình lần đầu được thực thi
            {
                currentProcess.iStart=curTime;
                currentProcess.iResponse=currentProcess.iStart-currentProcess.iArrival;
                currentProcess.iBurst-=q; // tg con lai cua process
                curTime+=q;
                while(iReady < n and curTime>=Input[iReady].iArrival)
                {
                    que.push(Input[iReady]);
                    iReady++;
                }
                que.push(currentProcess);
            }
            else if(currentProcess.iBurst<=q) //lần thực thi cuối của tiến trình
            {
                currentProcess.iFinish=curTime+currentProcess.iBurst;
                currentProcess.iTaT=currentProcess.iFinish-currentProcess.iArrival;
                currentProcess.iWaiting=currentProcess.iTaT-remain_burst[currentProcess.iPID];
                curTime+=currentProcess.iBurst;
                //cập nhật lại burst time ban đầu cho process
                currentProcess.iBurst=remain_burst[currentProcess.iPID];
                Terminated.push_back(currentProcess);
                iTerminated++;  
            }
            else // trường hợp process thực thi lần 2 trở đi nhưng chưa phải lần cuối (burst>q)
            {
                currentProcess.iBurst-=q;
                curTime+=q;
                while(iReady < n and curTime>=Input[iReady].iArrival)
                {
                    que.push(Input[iReady]);
                    iReady++;
                }
                que.push(currentProcess);
            }
        }
        else 
        {
            curTime++;
        }
    }
}

void calculate(double &awt,double &art, double &atat,int n, vector<PCB>Terminated)
{
    for(int i=0;i<n;i++)
    {
        awt+=Terminated[i].iWaiting;
        art+=Terminated[i].iResponse;
        atat+=Terminated[i].iTaT;
    }
    awt/=n;
    art/=n;
    atat/=n;
}

int main()
{
    vector<PCB> Input; 
    
    int n,q;
    cout<<"Enter the number of Process and Quantum time: ";
    cin>>n>>q;
    inputProcess(n,Input);
    // sap xep cac process theo thu tu tang dan arrival time
    sort(Input.begin(),Input.end(),cmp);

    int iReady = 0, iTerminated =0;
    vector<PCB> Terminated;
    execute(n,q,Input,Terminated,iReady,iTerminated);

    //sap xep cac process theo thu tu tang dan PID
    sort(Terminated.begin(),Terminated.end(),cmp2);
    outputProcess(n,Terminated);
    double AWT=0,ART=0,ATaT=0;
    //Hàm tinh thời gian trung bình
    calculate(AWT,ART,ATaT,n,Terminated);
    cout<<"The avarage of Waiting Time: "<<AWT<<endl;
    cout<<"The avarage of Response Time: "<<ART<<endl;
    cout<<"The avarage of Turnaround Time: "<<ATaT<<endl;
    return 0;
}


