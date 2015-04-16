/******************************************************************************
* Author: Anthony Morast
* CSC 456 - Operating Systems
* Dr. Christer Karlsson, Spring 2015
*
* This header file will contain all methods to simulate CPU processes
* scheduling. The methods simulated will be Round Robin, Priority, and Shortest
* Job First scheduling. 
*
* Format of processes array: 
    processes[i][0] = burst time
    processes[i][1] = arrival time 
    processes[i][2] = priortiy (defualt = 0)
    
******************************************************************************/

/******************************************************************************
                        Libraries, Classes, etc. 
******************************************************************************/
#include <iostream>
#include <stdio.h>
#include "process.h"

using namespace std;

class ProcessScheduler 
{
    public:
        void round_robin ( Process *processes, int quantum, int num_procs );
        void priority ( Process *processes, int num_procs );
        void shortest_job_first ( Process *processes, int num_procs );
    private:
        Process *burst_sort ( Process *processes, int num_procs );
        Process *priority_sort ( Process *processes, int num_procs );
        Process *arrival_sort ( Process *processes, int num_procs);
};

/******************************************************************************
                        Process Scheduling Algorithms 
******************************************************************************/

/******************************************************************************
* Author: Anthony Morast
*
* This function takes an array of processes and a time quantum indicating how
* long each process is alotted the CPU. The algorithm begins by sorting the 
* input array of processes by their arrival time which is more convienient for
* the remainder of the algorithm and by determining how much time all of the 
* processes will take. Then each clock_tick is cycled through using an new 
* array of queued processes. If a new process arrives it is added to the end of
* the queue. If the running process is finished or the quantum has expired the
* processes are all shifted in the queue to a higher position and the process
* is re-added to the end of the list if it needs further execution otherwise
* it is deleted.
******************************************************************************/
void ProcessScheduler::round_robin ( Process* processes, int quantum, int num_procs )
{
    int total_time = 0;

    // sort by arrival time? 
    // preemption and priorities?

    // get total time
    for (int i = 0; i < num_procs; i++)
       total_time += processes[i].burst_time; 

    // sort by arrival time
    processes = arrival_sort(processes, num_procs);
    Process *queued_procs = new Process[num_procs]; 

    int time = 0, running_time = 0;
    int queued_procs_size = 0;

    for (time = 0; time < total_time; time++)
    {
        // add processes that just arrived to end of queue
        for (int i = 0; i < num_procs; i++)
        {
            if (processes[i].arrival_time == time)
            {
                queued_procs[queued_procs_size] = processes[i];
//                printf("Added process %d to queue with burst time %d and arrival time %d at cpu time %d\n",
//                        processes[i].process_id, processes[i].burst_time, processes[i].arrival_time, time);
                queued_procs_size++;
            }
        }
        if (queued_procs_size > 0)  // if there are processes running or need be ran
        {
            queued_procs[0].burst_time--;
            running_time++;
//            printf("Process %d running at time %d with %d burst time remaining.\n", queued_procs[0].process_id, time, queued_procs[0].burst_time);
            // if the time quantum is up OR the process has completed
            if ( running_time % quantum == 0 || queued_procs[0].burst_time == 0 )
            {
                Process temp = queued_procs[0];
                // move all processes up in the queue
                for (int i = 1; i < queued_procs_size; i++)
                    queued_procs[i-1] = queued_procs[i];
                //if the processes is done, remove it
                if ( temp.burst_time == 0 )
                {
//                    printf("Process %d has finished at time %d\n", temp.process_id, time);
                    running_time = 0;
                    queued_procs_size--;
                }
                else // put it at the end
                {
                    queued_procs[queued_procs_size-1] = temp;
                }
            }
        }
       // printf("Queue at time %d: ", time);
       // for (int i = 0; i < queued_procs_size; i++)
       //     cout << queued_procs[i].process_id + " ";
       // cout << endl;
    }

    return;
}

/******************************************************************************
* Author: Anthony Morast
*
* priority takes an array of processes and their corresponding priorities. The
* method will implement the Priority scheduling algorithm. ....
******************************************************************************/
void ProcessScheduler::priority ( Process *processes, int num_procs )
{

}

/******************************************************************************
* Author: Anthony Morast
*
* shortest_job_first will take an array of processes to be scheduled. The
* array should contain the process length in an index. ....
******************************************************************************/
void ProcessScheduler::shortest_job_first ( Process *processes, int num_procs )
{
    processes = arrival_sort(processes, num_procs);  // sort by arrival time
    Process *q_procs = new Process[num_procs];
    int total_time = 0;
    int q_size = 0;
    Process temp;

    for (int i = 0; i < num_procs; i++)
        total_time += processes[i].burst_time;

    for (int time = 0; time < total_time; time++)
    {
        bool higher_priority = false;
        for (int i = 0; i < num_procs; i++)
        {
            if (processes[i].arrival_time == time)
            {
                q_procs[q_size] = processes[i];     // add process to list
                q_size++;                           // increment size
//                printf("Process %d added at time %d\n", q_procs[q_size-1].process_id, time);
            }
        }        
        if (q_size > 0)
        {
            q_procs[0].burst_time--;
            if (q_procs[0].burst_time <= 0)
            {
                for (int j = 1; j < q_size; j++) // move procs up in queue
                    q_procs[j-1] = q_procs[j];
                q_size--;          
            }

            // re order by job length
            q_procs = burst_sort (q_procs, q_size);
            for (int k = 0; k < q_size; k++)
//                printf("Process: %d   Priority: %d\n", q_procs[k].process_id, q_procs[k].priority);
        }        
    }

//    delete []processes;
//    delete []q_procs;

    return;
}

/******************************************************************************
* Author: Anthony Morast
*
* burst_sort sorts the processes in order from shortest to longest for the Shortest
* Job First scheduling algorithm. 
******************************************************************************/
Process* ProcessScheduler::burst_sort ( Process *processes, int num_procs )
{ 
    Process *sorted_procs = new Process[num_procs];
    for (int i = 0; i < num_procs; i++)
    {
        sorted_procs[i].burst_time = processes[i].burst_time;
        sorted_procs[i].arrival_time = processes[i].arrival_time;
        sorted_procs[i].priority = processes[i].priority;
        sorted_procs[i].process_id = processes[i].process_id;
    }
 
    int i, j;
    Process temp;
    for (i = 1; i < num_procs; i++)
    {
        for (j = i; j >= 1; j--)
        {
            if (sorted_procs[j].priority > sorted_procs[j-1].priority)
            {   
                temp = sorted_procs[j];
                sorted_procs[j] = sorted_procs[j-1];
                sorted_procs[j-1] = temp;
            }
            else break;
        }        
    }

    for (i = 0; i < num_procs; i++)
        processes[i] = sorted_procs[i];

    delete []sorted_procs;
    return processes;
}

/*****************************************************************************
* Author: Anthony Morast
*
* priority_sort sorts the list of processes based on their priority. 
******************************************************************************/
Process* ProcessScheduler::priority_sort ( Process *processes, int num_procs )
{
    Process *sorted_procs = new Process[num_procs];
    for (int i = 0; i < num_procs; i++)
    {
        sorted_procs[i].burst_time = processes[i].burst_time;
        sorted_procs[i].arrival_time = processes[i].arrival_time;
        sorted_procs[i].priority = processes[i].priority;
        sorted_procs[i].process_id = processes[i].process_id;
    }
    
    return sorted_procs;
} 

/******************************************************************************
* Author: Anthony Morast
*
* A sort to order the processes by arrival time as that's when they will be
* added to the queue
******************************************************************************/
Process *ProcessScheduler::arrival_sort (Process *processes, int num_procs)
{ 
    Process *sorted_procs = new Process[num_procs];

    for (int i = 0; i < num_procs; i++)
    {
        sorted_procs[i].burst_time = processes[i].burst_time;
        sorted_procs[i].arrival_time = processes[i].arrival_time;
        sorted_procs[i].priority = processes[i].priority;
        sorted_procs[i].process_id = processes[i].process_id;
    }


    int i, j;
    Process temp;
    for ( i = 1; i < num_procs; i++)
    {
        for (j = i; j >= 1; j--)
        {
            if (sorted_procs[j].arrival_time < sorted_procs[j-1].arrival_time)
            {
                temp = sorted_procs[j];
                sorted_procs[j] = sorted_procs[j-1];
                sorted_procs[j-1] = temp;
            }
            else break;
        }
    }
  
    return sorted_procs;
}
