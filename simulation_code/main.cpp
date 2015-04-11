#include <iostream>
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
    
    /*  IMPORTANT: for anyone editting this file, I was hoping to have the processes
        array have the following format for the first 3 indicies per processes:
            processes[i][0] = burst_time
            processes[i][1] = arrival time
            processes[i][2] = priority (default = 0)
        This is what I've based my class on so far, if we need to change I can, just
        a preference. Anything you might need in this array could be in indicies
        3-inf.  Thanks! 
    */
    int **processes;
    ProcessScheduler scheduler;
    
    return 0;
}
