#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
#include <bits/stdc++.h>
using namespace std;
const int num_processes = 7;
struct Process {
    int id;
    std::string name;
    int arrival_time;
    int burst_time;
    int comes_back_after;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    bool cheack = false;
    int q ;
    int currnt_time_priroty;
    int LastArrivalTime;

}Process;
// to add on wait or ready as sort burst time
struct comp_burst {
    bool operator()(struct Process const& p1, struct Process const& p2)
    {
        return p1.burst_time > p2.burst_time;
    }
};
// to add on wait or ready as sort priototy
struct comp_priototy {
    bool operator()(struct Process const& p1, struct Process const& p2)
    {
        if(p1.priority != p2.priority) return p1.priority > p2.priority;
        else return p1.LastArrivalTime > p2.LastArrivalTime; // if priority equals then compare arrival time first come first execute
    }
};
// Function to (First_Come_First_Served {non primative}) scheduling
void First_Come_First_Served (struct Process processes[]) {
    std::queue<struct Process> ready, wait;
    int totel_wait = 0, totel_turn = 0, currnt_time = 0, totalWaitingTime = 0, totalTurnaroundTime = 0;
    int remaining[num_processes], come_back[num_processes] , time_readyQueu[num_processes] , time_waitQueu [num_processes];
    struct Process currnt_proc ;
    int id1 , id2;
    bool flag = false ;
    // copy burst time && come back && and initial arrays
    for (int i = 0; i < num_processes; i++) {
        remaining[i] = processes[i].burst_time;
        come_back[i] = processes[i].comes_back_after;
        time_readyQueu[i]=0;
        time_waitQueu[i]=0;
    }
    std::cout << "Gantt Chart for First_Come_First_Served : \n  " << std::endl;
    // main loop arrive to 200 second
    while (currnt_time < 200) {
        std::queue<struct Process> temprory;
        // incremant wait time +1 for all process in ready queue
        while (!ready.empty()){
            struct Process currnt_redy = ready.front();
            ready.pop();
            currnt_redy.waiting_time+=1;
            temprory.push(currnt_redy);
        }
        ready=temprory; // move all queue temp to readyqueue
        while (!temprory.empty()) temprory.pop();
        // to Reduce by 1 from all come back after in wait queue
        while (!wait.empty()){
            struct Process currnt_wait = wait.front();
            wait.pop();
            // to calculate time of each process in wait queue
            time_waitQueu[currnt_wait.id]+=1;
            currnt_wait.comes_back_after-=1;
            // if come back equal zero we must return it to ready queue
            if(currnt_wait.comes_back_after==0){
                // give it main come back value
                currnt_wait.comes_back_after=come_back[currnt_wait.id];
                ready.push(currnt_wait);
            }else
                temprory.push(currnt_wait);
        }
        wait=temprory; // move all queue temp to readyqueue
        while (!temprory.empty()) temprory.pop();
        // save any process arrive in ready queue
        for (int j = 0; j < num_processes; j++) {
            if (processes[j].arrival_time <= currnt_time && processes[j].cheack == false){
                ready.push(processes[j]);
                processes[j].cheack = true;
            }
        }
        if(currnt_time==0) {
            currnt_proc=ready.front();
            ready.pop();
        }
        // if process end then give it main burst time and add it to wait queue
        if(currnt_proc.burst_time==0){
            currnt_proc.burst_time=remaining[currnt_proc.id];
            wait.push(currnt_proc);
            std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
            flag = true; // flag to share is cpu empty
        }
            // if cpu empty and ready have processs take the next process
            if(flag == true && !ready.empty()){
                flag= false;
                currnt_proc=ready.front();
                ready.pop();
                // to calculate time of each process in ready queue
                    time_readyQueu[currnt_proc.id]+=currnt_proc.waiting_time;
                // to calculate totel waiting time && turnaround time
                    totel_wait+=currnt_proc.waiting_time;
                    totel_turn+=currnt_proc.waiting_time+time_waitQueu[currnt_proc.id];
                    currnt_proc.waiting_time=0;
                     time_waitQueu[currnt_proc.id]=0;
            }
        currnt_proc.burst_time-=1; // decremant burst for each second for current process
        currnt_time++;
    }
    std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
    cout<<"\n";
    // add cpu time for turn around (time
    totel_turn+=200;
    std::cout << "\n";
    std::cout << "Average Waiting Time = " << static_cast<float>(totel_wait) / num_processes << std::endl;
    std::cout << "Average Turnaround Time = " << static_cast<float>(totel_turn) / num_processes << std::endl;
    // initilization for array agine
    for(int i = 0;i < num_processes; i++) processes[i].cheack = false;
    cout<<"\n \n";
}
// Function to (Shortest_Job_First {non primative}) scheduling
void Shortest_Job_First (struct Process processes[]){
    std:: priority_queue<struct Process,vector<struct Process>,comp_burst> ready, wait ;
    set <int> check_done; // set to save num of process execute without repate
    int totel_wait = 0, totel_turn = 0, currnt_time = 0, totalWaitingTime = 0, totalTurnaroundTime = 0;
    int remaining[num_processes], come_back[num_processes] , time_readyQueu[num_processes]  , time_waitQueu[num_processes] ;
    struct Process currnt_proc ;
    bool flag = false ;
    // copy burst time && come back && and initial arrays
    for (int i = 0; i < num_processes; i++) {
        remaining[i] = processes[i].burst_time;
        come_back[i] = processes[i].comes_back_after;
        time_readyQueu[i]=0;
        time_waitQueu[i]=0;
    }
    std::cout << "Gantt Chart for Shortest_Job_First  :  \n" << std::endl;
    while (currnt_time < 200) {
        std::priority_queue<struct Process,vector<struct Process>,comp_burst> temprory;
        // incremant wait time +1 for all process in ready queue
        while (!ready.empty()){
            struct Process currnt_redy = ready.top();
            ready.pop();
            currnt_redy.waiting_time+=1;
            temprory.push(currnt_redy);
        }
        ready=temprory; // move all queue temp to ready queue
        while (!temprory.empty()) temprory.pop();
        // to Reduce by 1 from all come back after in wait queue
        while (!wait.empty()){
            struct Process currnt_wait = wait.top();
            wait.pop();
            // to calculate time of each process in wait queue
            time_waitQueu[currnt_wait.id]+=1;
            currnt_wait.comes_back_after-=1;
            // if come back equal zero we must return it to ready queue
            if(currnt_wait.comes_back_after==0){
                // give it main come back value
                currnt_wait.comes_back_after=come_back[currnt_wait.id];
                ready.push(currnt_wait);
            }else
                temprory.push(currnt_wait);
        }
        wait=temprory; // move all queue temp to readyqueue
        while (!temprory.empty()) temprory.pop();
        // save any process arrive in ready queue
        for (int j = 0; j < num_processes; j++) {
            if (processes[j].arrival_time <= currnt_time && processes[j].cheack == false){
                ready.push(processes[j]);
                processes[j].cheack = true;
            }
        }
        if(currnt_time==0) {
            currnt_proc=ready.top();
            ready.pop();
            // to in last know unique number of process has executed
            check_done.insert(currnt_proc.id);
        }
        if(currnt_proc.burst_time==0 ){
            currnt_proc.burst_time=remaining[currnt_proc.id];
            wait.push(currnt_proc);
            std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
            flag = true; // flag to share is cpu empty
        }
        // if cpu empty and ready have processs take the next proces
        if(flag == true && !ready.empty()){
            flag = false;
            currnt_proc=ready.top();
            ready.pop();
            // to calculate time of each process in ready queue
            time_readyQueu[currnt_proc.id]+=currnt_proc.waiting_time;
            // to calculate totel waiting time && turnaround time
            totel_wait+=currnt_proc.waiting_time;
            totel_turn+=currnt_proc.waiting_time+time_waitQueu[currnt_proc.id];
            currnt_proc.waiting_time=0;
            time_waitQueu[currnt_proc.id]=0;
            check_done.insert(currnt_proc.id);
        }
        currnt_proc.burst_time-=1; // decremant burst for each second for current process
        currnt_time++;
    }
    std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
    cout<<"\n ";
    // add cpu time for turn around (time
    totel_turn+=200;
    std::cout << "\n";
    std::cout << "Average Waiting Time = " << static_cast<float>(totel_wait) / check_done.size() << std::endl;
    std::cout << "Average Turnaround Time = " << static_cast<float>(totel_turn) / check_done.size() << std::endl;
    // initilization for array agine
    for(int i = 0;i < num_processes; i++) processes[i].cheack = false;
    cout<<"\n \n";
}
// Function to (Shortest_Remaining_Time_First {primitive}) scheduling
void Shortest_Remaining_Time_First (struct Process processes[]){
    std:: priority_queue<struct Process,vector<struct Process>,comp_burst> ready, wait ;
    set <int> check_done; // set to save num of process execute without repate
    int totel_wait = 0, totel_turn = 0, currnt_time = 0, totalWaitingTime = 0, totalTurnaroundTime = 0;
    int remaining[num_processes], come_back[num_processes] , time_readyQueu[num_processes] , time_waitQueu[num_processes] ;
    struct Process currnt_proc ;
    bool k = false , flag = false ;
    // copy burst time && come back && and initial arrays
    for (int i = 0; i < num_processes; i++) {
        remaining[i] = processes[i].burst_time;
        come_back[i] = processes[i].comes_back_after;
        time_readyQueu[i]=0;
        time_waitQueu[i]=0;
    }
    std::cout << "Gantt Chart for Shortest_Remaining_Time_First  : \n " << std::endl;
    while (currnt_time<200){
        std::priority_queue<struct Process,vector<struct Process>,comp_burst> temprory;
        // incremant wait time +1 for all process in ready queue
        while (!ready.empty()){
            struct Process currnt_redy = ready.top();
            ready.pop();
            currnt_redy.waiting_time+=1;
            temprory.push(currnt_redy);
        }
        ready=temprory; // move all queue temp to readyqueue
        while (!temprory.empty()) temprory.pop();
        // to Reduce by 1 from all come back after in wait queue
        while (!wait.empty()){
            struct Process currnt_wait = wait.top();
            wait.pop();
            currnt_wait.comes_back_after-=1;
            // to calculate time of each process in wait queue
            time_waitQueu[currnt_wait.id]+=1;
            // if come back equal zero we must return it to ready queue
            if(currnt_wait.comes_back_after==0){
                // give it main come back value
                currnt_wait.comes_back_after=come_back[currnt_wait.id];
                ready.push(currnt_wait);
            }else
                temprory.push(currnt_wait);
        }
        wait=temprory; // move all queue temp to readyqueue
        while (!temprory.empty()) temprory.pop();
        // save any process arrive in ready queue
        for (int j = 0; j < num_processes; j++) {
            if (processes[j].arrival_time <= currnt_time && processes[j].cheack == false){
                ready.push(processes[j]);
                processes[j].cheack = true;
            }
        }
        if(currnt_time==0) {
            currnt_proc=ready.top();
            ready.pop();
            check_done.insert(currnt_proc.id);
        }
        if(currnt_proc.burst_time==0 ){
            currnt_proc.burst_time=remaining[currnt_proc.id];
            wait.push(currnt_proc);
            std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
            flag = true; // flag to share is cpu empty
            k= true;
        }
        //  if k= fales then in process not end then we check and compare burst time to change it
        if(!ready.empty() && k == false){
            struct Process temp = ready.top();
            if(currnt_proc.burst_time>temp.burst_time){
                ready.pop();
                std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
                ready.push(currnt_proc);
                currnt_proc=temp;
                // to calculate time of each process in ready queue
                time_readyQueu[currnt_proc.id]+=currnt_proc.waiting_time;
                // to calculate totel waiting time && turnaround time
                totel_wait+=currnt_proc.waiting_time;
                totel_turn+=currnt_proc.waiting_time+time_waitQueu[currnt_proc.id];
                currnt_proc.waiting_time=0;
                time_waitQueu[currnt_proc.id]=0;
                check_done.insert(currnt_proc.id);
            }
        }
        // if cpu empty and ready have processs take the next proces
        if(flag == true && !ready.empty()){
            k= false;
            flag = false ;
            currnt_proc=ready.top();
            ready.pop();
            // to calculate time of each process in ready queue
            time_readyQueu[currnt_proc.id]+=currnt_proc.waiting_time;
            // to calculate totel waiting time && turnaround time
            totel_wait+=currnt_proc.waiting_time;
            totel_turn+=currnt_proc.waiting_time+time_waitQueu[currnt_proc.id];
            currnt_proc.waiting_time=0;
            time_waitQueu[currnt_proc.id]=0;
            check_done.insert(currnt_proc.id);
        }
        currnt_proc.burst_time-=1; // decremant burst for each second for current process
        currnt_time++;
    }
    std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
    cout<<"\n";
    // add cpu time for turn around (time
    totel_turn+=200;
    std::cout << "\n";
    std::cout << "Average Waiting Time = " << static_cast<float>(totel_wait) / check_done.size() << std::endl;
    std::cout << "Average Turnaround Time = " << static_cast<float>(totel_turn) / check_done.size() << std::endl;
    // initilization for array agine
    for(int i = 0;i < num_processes; i++) processes[i].cheack = false;
    cout<<"\n";
}
// Function to (Round Robin, with q = 5) scheduling
void Round_Robin (struct Process processes[]){
    std::queue<struct Process> ready, wait;
    int totel_wait = 0, totel_turn = 0, currnt_time = 0, totalWaitingTime = 0, totalTurnaroundTime = 0;
    int remaining[num_processes], come_back[num_processes] , time_readyQueu [num_processes] , time_waitQueu[num_processes] ;
    struct Process currnt_proc ;
    bool flag = false;
    // copy burst time && come back && and initial arrays
    for (int i = 0; i < num_processes; i++) {
        remaining[i] = processes[i].burst_time;
        come_back[i] = processes[i].comes_back_after;
        time_readyQueu[i]=0;
        time_waitQueu[i]=0;
    }
    std::cout << "Gantt Chart for Round Robin : \n" << std::endl;
    while (currnt_time<200){
        std::queue<struct Process> temprory;
        // incremant wait time +1 for all process in ready queue
        while (!ready.empty()){
            struct Process currnt_redy = ready.front();
            ready.pop();
            currnt_redy.waiting_time+=1;
            temprory.push(currnt_redy);
        }
        ready=temprory; // move all queue temp to readyqueue
        while (!temprory.empty()) temprory.pop();
        // to Reduce by 1 from all come back after in wait queue
        while (!wait.empty()){
            struct Process currnt_wait = wait.front();
            wait.pop();
            currnt_wait.comes_back_after-=1;
            // to calculate time of each process in wait queue
            time_waitQueu[currnt_wait.id]+=1;
            // if come back equal zero we must return it to ready queue
            if(currnt_wait.comes_back_after==0){
                // give it main come back value
                currnt_wait.comes_back_after=come_back[currnt_wait.id];
                ready.push(currnt_wait);
            }else
                temprory.push(currnt_wait);
        }
        wait=temprory; // move all queue temp to readyqueue
        while (!temprory.empty()) temprory.pop();
        // save any process arrive in ready queue
        for (int j = 0; j < num_processes; j++) {
            if (processes[j].arrival_time <= currnt_time && processes[j].cheack == false){
                ready.push(processes[j]);
                processes[j].cheack = true;
            }
        }
        if(currnt_time==0) {
            currnt_proc=ready.front();
            ready.pop();
        }
        if(currnt_proc.burst_time==0 ){
            currnt_proc.burst_time=remaining[currnt_proc.id];
            currnt_proc.q=5;
            wait.push(currnt_proc);
            std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
            flag = true; // flag to share is cpu empty
        }
        // if q has end and burst time not finish then return main q for it and add it to readuQUeue
        if(currnt_proc.q==0 && currnt_proc.burst_time!=0){
            currnt_proc.q=5;
            ready.push(currnt_proc);
            std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
            currnt_proc=ready.front();
            ready.pop();
            // to calculate time of each process in ready queue
            time_readyQueu[currnt_proc.id]+=currnt_proc.waiting_time;
            // to calculate totel waiting time && turnaround time
            totel_wait+=currnt_proc.waiting_time;
            totel_turn+=currnt_proc.waiting_time+time_waitQueu[currnt_proc.id];
            currnt_proc.waiting_time=0;
            time_waitQueu[currnt_proc.id]=0;
        }
        // if cpu empty and ready have processs take the next proces
        if(flag == true && !ready.empty()){
            flag = false;
            currnt_proc=ready.front();
            ready.pop();
            // to calculate time of each process in ready queue
            time_readyQueu[currnt_proc.id]+=currnt_proc.waiting_time;
            // to calculate totel waiting time
            totel_wait+=currnt_proc.waiting_time;
            totel_turn+=currnt_proc.waiting_time+time_waitQueu[currnt_proc.id];
            currnt_proc.waiting_time=0;
            time_waitQueu[currnt_proc.id]=0;
        }
        currnt_proc.burst_time-=1; // decremant burst for each second for current process
        currnt_proc.q-=1; // decremant q for each second  for current process
        currnt_time++;
    }
    std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
    cout<<"\n";
    // add cpu time for turn around (time
    totel_turn+=200;
    std::cout << "\n";
    std::cout << "Average Waiting Time = " << static_cast<float>(totel_wait) / num_processes << std::endl;
    std::cout << "Average Turnaround Time = " << static_cast<float>(totel_turn) / num_processes << std::endl;
    // initilization for array agine
    for(int i = 0;i < num_processes; i++) processes[i].cheack = false;
    cout<<"\n \n";
}
// Non_Preemptive_Priority Scheduling, with aging
void Non_Preemptive_Priority (struct Process processes[] ){
    std::priority_queue<struct Process,vector<struct Process>,comp_priototy> ready, temprory;
    queue<struct Process>  wait;
    set <int> check_done; // set to save num of process execute without repate
    int totel_wait = 0, totel_turn = 0, currnt_time = 0, totalWaitingTime = 0, totalTurnaroundTime = 0 , currnt_time_priototy=0;
    int remaining[num_processes], come_back[num_processes] , priority[num_processes] ,time_readyQueu[num_processes] , time_waitQueu[num_processes] ;
    struct Process currnt_proc ;
    bool flag  = false;
    // copy burst time && come back && priority && arrival time && initilazition arrays
    for (int i = 0; i < num_processes; i++) {
        remaining[i] = processes[i].burst_time;
        come_back[i] = processes[i].comes_back_after;
        processes[i].LastArrivalTime = processes[i].arrival_time;
        priority[i]=processes[i].priority;
        time_readyQueu[i]=0;
        time_waitQueu[i]=0;
    }
    std::cout << "Gantt Chart for Non_Preemptive_Priority  : \n" << std::endl;
    while (currnt_time<200) {
        // incremant wait time +1 for all process in ready queue
        while (!ready.empty()){
            struct Process currnt_redy = ready.top();
            ready.pop();
            currnt_redy.waiting_time+=1;
            currnt_redy.currnt_time_priroty+=1;
            // time equal 5 time units then decremant priority and give zero for time *(time for priority not current time)*
            if(currnt_redy.currnt_time_priroty==5 && currnt_redy.priority!=0) {
                // if priority not equal zero decremant it
                currnt_redy.priority -=1;
                currnt_redy.currnt_time_priroty=0;
            }else if (currnt_redy.currnt_time_priroty==5 && currnt_redy.priority==0){
                // if priority  equal zero not decremant it
                currnt_redy.currnt_time_priroty=0;
            }
            temprory.push(currnt_redy);
        }
        ready=temprory; // move all queue temp to ready queue
        while (!temprory.empty()) temprory.pop();
        queue<struct Process>  temp_wait;
        // to Reduce by 1 from all come back after in wait queue
        while (!wait.empty()){
            struct Process currnt_wait = wait.front();
            wait.pop();
            // to calculate time of each process in wait queue
            time_waitQueu[currnt_wait.id]+=1;
            currnt_wait.comes_back_after-=1;
            // if equal zero we must return it to ready queue
            if(currnt_wait.comes_back_after==0){
                // give it main come back value
                currnt_wait.comes_back_after=come_back[currnt_wait.id];
                currnt_wait.currnt_time_priroty=0; // make time for prioity zero to cal form start
                // give new arrival time to compare it when the priority equal
                currnt_wait.LastArrivalTime = currnt_time;
                ready.push(currnt_wait);
            }else
                temp_wait.push(currnt_wait);
        }
        wait=temp_wait; // move all queue temp to readyqueue
        while (!temprory.empty()) temprory.pop();
        // save any process arrive in ready queue
        for (int j = 0; j < num_processes; j++) {
            if (processes[j].arrival_time <= currnt_time && processes[j].cheack == false){
                ready.push(processes[j]);
                processes[j].cheack = true;
            }
        }
        if(currnt_time==0) {
            currnt_proc=ready.top();
            ready.pop();
            check_done.insert(currnt_proc.id);
        }
        // if process end burst time then give it main priority and burst time and add it to wait queue
        if(currnt_proc.burst_time==0){
            currnt_proc.burst_time=remaining[currnt_proc.id];
            currnt_proc.priority= priority[currnt_proc.id];
            std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
            wait.push(currnt_proc);
            flag = true; // flag to share is cpu empty
        }
        // if cpu empty and ready have processs take the next proces
        if(flag == true && !ready.empty()){
            flag = false;
            currnt_proc=ready.top();
            ready.pop();
            currnt_proc.currnt_time_priroty=0;
            // to calculate time of each process in ready queue
            time_readyQueu[currnt_proc.id]+=currnt_proc.waiting_time;
            // to calculate totel waiting time && turnaround time
            totel_wait+=currnt_proc.waiting_time;
            totel_turn+=currnt_proc.waiting_time+time_waitQueu[currnt_proc.id];
            currnt_proc.waiting_time=0;
            time_waitQueu[currnt_proc.id]=0;
            check_done.insert(currnt_proc.id);
        }
        currnt_proc.burst_time-=1; // decremant burst for each second for current process
        currnt_time++;
    }
    std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
    cout<<"\n";
    // add cpu time for turn around (time
    totel_turn+=200;
    std::cout << "\n";
    std::cout << "Average Waiting Time = " << static_cast<float>(totel_wait) / check_done.size() << std::endl;
    std::cout << "Average Turnaround Time = " << static_cast<float>(totel_turn) / check_done.size() << std::endl;
    // initilization for array agine
    for(int i = 0;i < num_processes; i++) processes[i].cheack = false;
    cout<<"\n \n";
}
// Preemptive_Priority Scheduling, with aging
void Preemptive_Priority (struct Process processes[] ) {
    std::priority_queue<struct Process,vector<struct Process>,comp_priototy> ready, temprory;
    queue<struct Process>  wait;
    set <int> check_done; // set to save num of process execute without repate
    int totel_wait = 0, totel_turn = 0, currnt_time = 0, totalWaitingTime = 0, totalTurnaroundTime = 0 , currnt_time_priototy=0;
    int remaining[num_processes], come_back[num_processes] , priority[num_processes] ,time_readyQueu[num_processes] , time_waitQueu[num_processes] ;
    struct Process currnt_proc ;
    bool flag  = false , k= false;
    // copy burst time && come back && priority && arrival time && initilazition arrays
    for (int i = 0; i < num_processes; i++) {
        remaining[i] = processes[i].burst_time;
        come_back[i] = processes[i].comes_back_after;
        processes[i].LastArrivalTime = processes[i].arrival_time;
        priority[i]=processes[i].priority;
        time_readyQueu[i]=0;
        time_waitQueu[i]=0;
    }
    std::cout << "Gantt Chart for Preemptive_Priority  : \n" << std::endl;
    while (currnt_time<200) {
        // incremant wait time +1 for all process in ready queue
        while (!ready.empty()){
            struct Process currnt_redy = ready.top();
            ready.pop();
            currnt_redy.waiting_time+=1;
            currnt_redy.currnt_time_priroty+=1;
            // time equal 5 time units then decremant priority and give zero for time *(time for priority not current time)*
            if(currnt_redy.currnt_time_priroty==5 && currnt_redy.priority!=0) {
                // if priority not equal zero decremant it
                currnt_redy.priority -=1;
                currnt_redy.currnt_time_priroty=0;
            }else if (currnt_redy.currnt_time_priroty==5 && currnt_redy.priority==0){
                // if priority  equal zero not decremant it
                currnt_redy.currnt_time_priroty=0;
            }
            temprory.push(currnt_redy);
        }
        ready=temprory; // move all queue temp to ready queue
        while (!temprory.empty()) temprory.pop();
        queue<struct Process>  temp_wait;
        // to Reduce by 1 from all come back after in wait queue
        while (!wait.empty()){
            struct Process currnt_wait = wait.front();
            wait.pop();
            // to calculate time of each process in wait queue
            time_waitQueu[currnt_wait.id]+=1;
            currnt_wait.comes_back_after-=1;
            // if equal zero we must return it to ready queue
            if(currnt_wait.comes_back_after==0){
                // give it main come back value
                currnt_wait.comes_back_after=come_back[currnt_wait.id];
                currnt_wait.currnt_time_priroty=0; // make time for prioity zero to cal form start
                // give new arrival time to compare it when the priority equal
                currnt_wait.LastArrivalTime = currnt_time;
                ready.push(currnt_wait);
            }else
                temp_wait.push(currnt_wait);
        }
        wait=temp_wait; // move all queue temp to readyqueue
        while (!temprory.empty()) temprory.pop();
        // save any process arrive in ready queue
        for (int j = 0; j < num_processes; j++) {
            if (processes[j].arrival_time <= currnt_time && processes[j].cheack == false){
                ready.push(processes[j]);
                processes[j].cheack = true;
            }
        }
        if(currnt_time==0) {
            currnt_proc=ready.top();
            ready.pop();
            check_done.insert(currnt_proc.id);
        }
        // if process end burst time then give it main priority and burst time and add it to wait queue
        if(currnt_proc.burst_time==0){
            currnt_proc.burst_time=remaining[currnt_proc.id];
            currnt_proc.priority= priority[currnt_proc.id];
            currnt_proc.currnt_time_priroty=0;
            std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
            wait.push(currnt_proc);
            flag = true; // flag to share is cpu empty
            k= true;
        }
        //  if k= fales then in process not end then we check and compare burst time to change it
        if (!ready.empty() && k == false) {
            struct Process temp = ready.top();
            if ( currnt_proc.priority > temp.priority){
                ready.pop();
                std::cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
                ready.push(currnt_proc);
                currnt_proc = temp;
                // to calculate time of each process in ready queue
               // time_readyQueu[currnt_proc.id]+=currnt_proc.waiting_time;
                // to calculate totel waiting time && turnaround time
                totel_wait+=currnt_proc.waiting_time;
                totel_turn+=currnt_proc.waiting_time+time_waitQueu[currnt_proc.id];
                currnt_proc.waiting_time=0;
                time_waitQueu[currnt_proc.id]=0;
                check_done.insert(currnt_proc.id);
            }
        }
        // if cpu empty and ready have processs take the next proces
        if(flag == true && !ready.empty()){
            flag = false;
            k= false;
            currnt_proc=ready.top();
            ready.pop();
            // to calculate time of each process in ready queue
            time_readyQueu[currnt_proc.id]+=currnt_proc.waiting_time;
            // to calculate totel waiting time && turnaround time
            totel_wait+=currnt_proc.waiting_time;
            totel_turn+=currnt_proc.waiting_time+time_waitQueu[currnt_proc.id];
            currnt_proc.waiting_time=0;
            time_waitQueu[currnt_proc.id]=0;
            check_done.insert(currnt_proc.id);
        }
        currnt_proc.burst_time-=1; // decremant burst for each second for current process
        currnt_time++;
    }
    std :: cout << "P" << currnt_proc.id + 1 << "_______ " << currnt_time << " ";
    cout<<"\n";
    // add cpu time for turn around (time
    totel_turn+=200;
    std::cout << "\n";
    std::cout << "Average Waiting Time = " << static_cast<float>(totel_wait) / check_done.size() << std::endl;
    std::cout << "Average Turnaround Time = " << static_cast<float>(totel_turn) / check_done.size() << std::endl;
    // initilization for array agine
    for(int i = 0;i < num_processes; i++) processes[i].cheack = false;
    cout<<"\n \n";
}
int main() {
    struct Process processes[] = {
            {0,"P1", 0, 10, 2, 3, 0, 0, 0, false,5,0},
            {1,"P2", 1, 8, 4, 2, 0, 0, 0,false,5,0},
            {2,"P3", 3, 14, 6, 3, 0, 0, 0,false,5,0},
            {3,"P4", 4, 7, 8, 1, 0, 0, 0,false,5,0},
            {4,"P5", 6, 5, 3, 0, 0, 0, 0,false,5,0},
            {5,"P6", 7, 4, 6, 1, 0, 0, 0,false,5,0},
            {6,"P7", 8, 6, 9, 2, 0, 0, 0,false,5,0},
    };
    cout << " scheduling CPU algorithms  ... \n \n ";
    First_Come_First_Served(processes);
    Shortest_Job_First(processes);
    Shortest_Remaining_Time_First(processes);
    Round_Robin(processes);
    Non_Preemptive_Priority(processes);
    Preemptive_Priority(processes);
    return 0;
}
