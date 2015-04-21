#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>

#include "memory_management.h"
#include "process_scheduler.h"
#include "page_replacement.h"

using namespace std;

void PrintHelp();

int main (int argc, char* argv[])
{
    // required args: #processes, quantum, #frames, #pages, 
    string help = "-help";
    if (argc > 1 && !help.compare(argv[1]))
    {
        PrintHelp();
        return 0;
    }
    if (argc <= 4) // or 4 or 5 or whatever
    {
        cout << "\nUsage: ./simulation <num_processes> <quantum> <num_frames> <num_pages>" << endl;
        cout << "or type ./simulation -help for help" << endl << endl;
        return 0;
    }    

    srand(time(0));
    int num_procs = 10;
    int quantum = 3;
    int num_frames = 3;
    int num_pages = 10;

    try
    {
        num_procs = stoi(argv[1]);
        quantum = stoi(argv[2]);
        num_frames = stoi(argv[3]);
        num_pages = stoi(argv[4]);
    }
    catch (const exception &e)
    {
        cout << "Exception! - \"" << e.what() << "\" while parsing input." << endl;
        throw;
    }
    catch (...)
    {
        cout << "Exception! - Default exception while parsing input" << endl;
        throw;
    }

    /******************* Process Scheduling *********************/    
    Process *processes;
    processes = new Process[num_procs]; 

    // generate the processes 
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
    
    // run round_robin
    scheduler.round_robin(processes, quantum, num_procs);
    cout << endl << endl;
    // run SJF preemptive
    scheduler.shortest_job_first(processes, num_procs, true);
    cout << endl << endl;
    // run SJF non-preemptive
    scheduler.shortest_job_first(processes, num_procs, false);
    cout << endl << endl;
    // run priority preemptive
    scheduler.priority(processes, num_procs, true);
    cout << endl << endl;
    // run priority non-preemptive
    scheduler.priority(processes, num_procs, false);
    cout << endl << endl;

    /***************** Page Replacement *******************/
    int num_page_accesses = rand() % (20) + 10;
    int *reference_string = new int[num_page_accesses];

    for (int i = 0; i < num_page_accesses; i++)
    {
        int access = rand() % num_pages;
        reference_string[i] = access;
    }

    fifo();
    optimal();
    lru();
    lfu(num_page_accesses, reference_string, num_pages, num_frames);
    second_chance(num_page_accesses, reference_string, num_pages, num_frames);
    clock_alg(num_page_accesses, reference_string, num_pages, num_frames);

    return 0;
}

void PrintHelp()
{
    cout << endl << "Simulation: Simulates process scheduling, memory management, and page replacement algorithms.\n" << endl;
    cout << "Arguments:\n";
    cout << "\tnum_procs - the number of processes to be run through the process scheduler." << endl;
    cout << "\tquantum - the time quantum used in round robin scheduling (how long will each process have before preemption" << endl;
    cout << "\tnum_frames - the number of frames to be used for page replacement (how many pages loaded at a time." << endl;
    cout << "\tnum_pages - the number of pages that will be loaded into memory at some point." << endl;
    cout << endl << "Information:\n";
    cout << "\tThis program will simulate multiple process scheduling, memory management unit, and page replacement algorithms.";
    cout << " The page replacement algorithms will use a maximum of 30 accesses and a minumum of 10 accesses. Each page is not garunteed";
    cout << " to be accessed.";
    cout << endl << endl;
}
