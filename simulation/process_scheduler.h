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
* long each process is alotted the CPU. .... 
******************************************************************************/
void ProcessScheduler::round_robin ( int** processes, int quantum, int num_procs )
{

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
}

/******************************************************************************
* Author: Anthony Morast
*
* burst_sort sorts the processes in order from shortest to longest for the Shortest
* Job First scheduling algorithm. 
******************************************************************************/
int** ProcessScheduler::burst_sort ( int **processes, int num_procs )
{

}

/*****************************************************************************
* Author: Anthony Morast
*
* priority_sort sorts the list of processes based on their priority. 
******************************************************************************/
int** ProcessScheduler::priority_sort ( int **processes, int num_procs )
{

} 
