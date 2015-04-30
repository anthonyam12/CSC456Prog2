#ifndef _MEMORY_MANAGEMENT_H_
#define _MEMORY_MANAGEMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "process.h"

using namespace std;

void hello();

class MMU
{

public:

    MMU();
    void Setup(vector<Process>& procs);
    unsigned int* Config_Page_Table(unsigned int memReq, unsigned int & size, unsigned int & fragmentation);
    bool Access_Memory(Process& proc, unsigned int mem); 
    void Terminate_Proc(Process& proc, unsigned int id);
    void Terminate_All_Procs(vector<Process>& procs);
    bool Create_Proc(Process& proc);
    void Create_Multiple_Procs(vector<Process>& procs);
    bool Is_Init();
    int Total_Fragmentation(vector<Process>& procs);

    void View_System_Info(vector<Process>& procs);
    void View_Process_Info(vector<Process>& procs);

    int virtualSpace;
    int physicalSpace;
    int pageSize;
    int numPages;
    int numFrames;
    vector<unsigned int> freeFrames;

private:
    bool initialized;
};

void memory_management_sim();
int custom_menu_prompt();
double random_float( double min, double max);
int random_int(int min, int max);
bool prompt_proc_req(int& numProcs, int& low, int& high);

#endif
