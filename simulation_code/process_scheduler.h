/******************************************************************************
* Author: Anthony Morast
* CSC 456 - Operating Systems
* Dr. Christer Karlsson, Spring 2015
*
* This header file will contain all methods to simulate CPU processes
* scheduling. The methods simulated will be Round Robin, Priority, and Shortest
* Job First scheduling.     
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
        void priority ( Process *processes, int num_procs, bool preempt );
        void shortest_job_first ( Process *processes, int num_procs, bool preempt );
    private:
        Process *burst_sort ( Process *processes, int num_procs );
        Process *priority_sort ( Process *processes, int num_procs );
        Process *arrival_sort ( Process *processes, int num_procs);
        float calculateTurnaround (Process *processes, int num_procs);
        float calculateResponse (Process *processes, int num_procs);
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
    Process *finished_procs = new Process[num_procs];

    int time = 0, running_time = 0;
    int queued_procs_size = 0, fin_proc_size = 0;

    cout << "ROUND ROBIN: " << endl;
    for (time = 0; time < total_time; time++)
    {
        // add processes that just arrived to end of queue
        for (int i = 0; i < num_procs; i++)
        {
            if (processes[i].arrival_time == time)
            {
                queued_procs[queued_procs_size] = processes[i];
                queued_procs_size++;
            }
        }
        if (queued_procs_size > 0)  // if there are processes running or need be ran
        {
            if (queued_procs[0].first_service)
            {
                queued_procs[0].response_time = time;
                queued_procs[0].first_service = false;
            }

            queued_procs[0].burst_time--;
            running_time++;
            printf("TIME: %d  PID: %d\n", time, queued_procs[0].process_id); 

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
                    running_time = 0;
                    queued_procs_size--;
                    temp.finish_time = time;
                    finished_procs[fin_proc_size] = temp;
                    fin_proc_size++;
                }
                else // put it at the end
                {
                    queued_procs[queued_procs_size-1] = temp;
                }
            }
        }
        else
            cout << "CPU IDLE AT TIME " << time << endl;
    }

    cout << endl << "Average Turnaround Time: " << calculateTurnaround(finished_procs, num_procs);
    cout << endl << "Average Response Time: " << calculateResponse(finished_procs, num_procs) << endl; 

    return;
}

/******************************************************************************
* Author: Anthony Morast
*
* priority takes an array of processes and their corresponding priorities. The
* method will implement the Priority scheduling algorithm. ....
******************************************************************************/
void ProcessScheduler::priority ( Process *processes, int num_procs, bool preempt )
{   
    Process* q_procs = new Process[num_procs];
    Process* fin_procs = new Process[num_procs];

    int fin_size = 0, q_size = 0;
    int time = 0;
    int total_time = 0;

    bool first_sort = true;

    for (int i = 0; i < num_procs; i++)
        total_time += processes[i].burst_time;

    for (time = 0; time < total_time; time++)
    {
        for (int i = 0; i < num_procs; i++)
        {
            if (processes[i].arrival_time == time)
            {
                q_procs[q_size] = processes[i];
                q_size++;
            }
            if (q_size > 0 && first_sort)
            {
                q_procs = priority_sort(q_procs, q_size);
                first_sort = false;
            }
        }
        if (q_size > 0)
        {
            if (q_procs[0].first_service)
            {
                q_procs[0].response_time = time;
                q_procs[0].first_service = false;
            }
            q_procs[0].burst_time--;
            printf("TIME: %d  PID: %d  PRIORITY: %d\n", time, q_procs[0].process_id, q_procs[0].priority);

            if (q_procs[0].burst_time <= 0)
            {
                q_procs[0].finish_time = time;
                fin_procs[fin_size] = q_procs[0];
                fin_size++;

                for (int j = 1; j < q_size; j++)
                    q_procs[j-1] = q_procs[j];
                q_size--;
               
                 if (!preempt)
                    q_procs = priority_sort(q_procs, q_size);
            }            

            if (preempt)
                q_procs = priority_sort(q_procs, q_size);
        }
        else 
            cout << "CPU Idle at time " << time << endl;
    }

    cout << endl << "Average Turnaround Time: " << calculateTurnaround(fin_procs, num_procs);
    cout << endl << "Average Response Time: " << calculateResponse(fin_procs, num_procs) << endl; 
    
    return;
}

/******************************************************************************
* Author: Anthony Morast
*
* shortest_job_first will take an array of processes to be scheduled. The
* array should contain the process length in an index. ....
******************************************************************************/
void ProcessScheduler::shortest_job_first ( Process *processes, int num_procs, bool preempt )
{
    processes = arrival_sort(processes, num_procs);  // sort by arrival time
    Process *q_procs = new Process[num_procs];
    Process *fin_procs = new Process[num_procs];

    int total_time = 0;
    int q_size = 0;
    int fin_size = 0;

    bool first_sort = true;

    Process temp;

    for (int i = 0; i < num_procs; i++)
        total_time += processes[i].burst_time;

    cout << "Shortest Job First: " << endl;
    for (int time = 0; time < total_time; time++)
    {
        for (int i = 0; i < num_procs; i++)
        {
            if (processes[i].arrival_time == time)
            {
                q_procs[q_size] = processes[i];     // add process to list
                q_size++;                           // increment size
            }
            if (q_size > 0 && first_sort)
            {
                first_sort = false;
                q_procs = burst_sort(q_procs, q_size);
            }
        }        
        if (q_size > 0)
        {
            if (q_procs[0].first_service)
            {
                q_procs[0].first_service = false;
                q_procs[0].response_time = time;
            }

            q_procs[0].burst_time--;
            printf ("TIME: %d  PID: %d  BURST REMAINING: %d\n", time, q_procs[0].process_id, q_procs[0].burst_time);

            if (q_procs[0].burst_time <= 0)
            {
                q_procs[0].finish_time = time;
                fin_procs[fin_size] = q_procs[0];
                fin_size++;

                for (int j = 1; j < q_size; j++) // move procs up in queue
                    q_procs[j-1] = q_procs[j];
                q_size--;         
                if (!preempt) 
                    q_procs = burst_sort (q_procs, q_size);
            }

            // re order by job length
            if (preempt)
                q_procs = burst_sort (q_procs, q_size);
        }   
        else
            cout << "CPU Idle at time " << time << endl;     
    }

    cout << endl << "Average Turnaround Time: " << calculateTurnaround(fin_procs, num_procs);
    cout << endl << "Average Response Time: " << calculateResponse(fin_procs, num_procs) << endl; 

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
        sorted_procs[i] = processes[i];
 
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
        sorted_procs[i] = processes[i];
    
    int i, j;
    Process temp;
    for (i = 1; i < num_procs; i++)
    {
        for (j=i; j >= 1; j--)
        {
            if (sorted_procs[j].priority < sorted_procs[j-1].priority)
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
        sorted_procs[i] = processes[i];

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
/******************************************************************************
* Author: Anthony Morast
* 
* This function calculates the average turnaround time of a set of processes.
******************************************************************************/
float ProcessScheduler::calculateTurnaround(Process *processes, int num_procs)
{
    float average = 0;

    for (int i = 0; i < num_procs; i++)
        average += (processes[i].finish_time - processes[i].arrival_time);
    average /= num_procs;

    return average;
}

/******************************************************************************
* Author: Anthony Morast
*
* This function calculates the average response time of a set of processes. 
******************************************************************************/
float ProcessScheduler::calculateResponse(Process *processes, int num_procs)
{
    float average = 0;

    for (int i = 0; i < num_procs; i++)
        average += processes[i].response_time;
    average /= num_procs;

    return average;
}
