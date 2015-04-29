#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdlib.h>

class Process
{
    public:
        //added functions
        Process();

        //Not added
        int arrival_time;
        int burst_time;
        int process_id;
        int priority;
        int finish_time;
        int response_time;
        bool first_service = true;

        //Added
        unsigned int mem_req;
        unsigned int* table;
        unsigned int table_size;
        unsigned int frag;

};

#endif
