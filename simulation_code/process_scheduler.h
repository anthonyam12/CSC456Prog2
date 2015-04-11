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
using namespace std;

class ProcessScheduler 
{
    public:
        void round_robin ( int **processes, int quantum, int num_procs );
        void priority ( int **processes, int num_procs );
        void shortest_job_first ( int **processes, int num_procs );
    private:
        int **burst_sort ( int **processes, int num_procs );
        int **priority_sort ( int **processes, int num_procs );
};

/******************************************************************************
                        Process Scheduling Algorithms 
******************************************************************************/

/******************************************************************************
* Author: Anthony Morast
*
* This function takes an array of processes and a time quantum indicating how
* long each process is alotted the CPU. 
******************************************************************************/
void ProcessScheduler::round_robin ( int** processes, int quantum, int num_procs )
{
    int total_time = 0;

    // sort by arrival time? 
    // preemption and priorities?

    // get total time
    for (int i = 0; i < num_procs; i++)
       total_time += processes[i][0]; 

    int k = 0;
    while (k <= total_time)
    {
        // go through all processes in queue
        for (int i = 0; i < num_procs; i++)
        {
            if ( processes[i][0] == 0 )
                continue;

            // time to add to CPU execution time (k) 
            int add_time = quantum;

            // if the processes needs the whole quantum, grant it
            if ( processes[i][0] > quantum )
                processes[i][0] -= quantum;
            else if ( processes[i][0] > 0 && processes[i][0] < quantum ) 
            {
                // if the processes doesn't need entire quantum, give part
                processes[i][0] = 0;
                add_time = processes[i][0];
            }

            // add time to CPU time
            k += add_time;
        }
    }
}

/******************************************************************************
* Author: Anthony Morast
*
* priority takes an array of processes and their corresponding priorities. The
* method will implement the Priority scheduling algorithm. ....
******************************************************************************/
void ProcessScheduler::priority ( int **processes, int num_procs )
{

}

/******************************************************************************
* Author: Anthony Morast
*
* shortest_job_first will take an array of processes to be scheduled. The
* array should contain the process length in an index. ....
******************************************************************************/
void ProcessScheduler::shortest_job_first ( int **processes, int num_procs )
{
    processes = burst_sort(processes, num_procs);
}

/******************************************************************************
* Author: Anthony Morast
*
* burst_sort sorts the processes in order from shortest to longest for the Shortest
* Job First scheduling algorithm. 
******************************************************************************/
int** ProcessScheduler::burst_sort ( int **processes, int num_procs )
{
    for (int i = 0; i < num_procs; i++)
    {

    }
    return processes;
}

/*****************************************************************************
* Author: Anthony Morast
*
* priority_sort sorts the list of processes based on their priority. 
******************************************************************************/
int** ProcessScheduler::priority_sort ( int **processes, int num_procs )
{

} 
