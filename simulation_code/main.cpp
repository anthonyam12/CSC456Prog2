#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>

#include "memory_management.h"
#include "process_scheduler.h"
#include "page_replacement.h"

using namespace std;

int main (int argc, char* argv[])
{
    if (argc < 3) // or 4 or 5 or whatever
    {
        cout << "\nSimulation requires the following parameters: .......\n " << endl;
        return 0;
    }    

    srand(time(0));
    bool preemptive = false;
    int num_procs = 10;
    int quantum = 3;
    
    Process *processes;
    processes = new Process[num_procs]; 
    cout << "Generated Process: " << endl;
    for (int i = 0; i < num_procs; i++)
    {
        processes[i].burst_time = rand() % 10 + 1;
        processes[i].arrival_time = rand() % 5;
        processes[i].priority = rand() % 5;
        processes[i].process_id = i;
        printf("Process %d has burst time %d, arrived at %d, and has priority %d.\n", i, processes[i].burst_time, processes[i].arrival_time, processes[i].priority);
    }
    cout << endl << endl;
    
    ProcessScheduler scheduler;
    scheduler.round_robin(processes, quantum, num_procs);
    cout << endl << endl;
    scheduler.shortest_job_first(processes, num_procs, preemptive);
    cout << endl << endl;
    scheduler.priority(processes, num_procs, preemptive);
    cout << endl << endl;

    return 0;
}
