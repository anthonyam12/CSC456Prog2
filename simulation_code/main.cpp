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
    
    /*  IMPORTANT: for anyone editting this file, I was hoping to have the processes
        array have the following format for the first 3 indicies per processes:
            processes[i][0] = burst_time
            processes[i][1] = arrival time
            processes[i][2] = priority (default = 0)
        This is what I've based my class on so far, if we need to change I can, just
        a preference. Anything you might need in this array could be in indicies
        3-inf.  Thanks! 
    */
    Process *processes;
    processes = new Process[10]; 
    for (int i = 0; i < 10; i++)
    {
        processes[i].burst_time = rand() % 15 + 1;
        processes[i].arrival_time = rand() % 5;
        processes[i].priority = rand() % 5;
        processes[i].process_id = i;
//        printf("Process %d has burst time %d, arrived at %d, and has priority %d.\n", i, processes[i].burst_time, processes[i].arrival_time, processes[i].priority);
    }
    int dummy;
//    cin >> dummy;

    ProcessScheduler scheduler;
    scheduler.round_robin(processes, 3, 10);
    return 0;
}
