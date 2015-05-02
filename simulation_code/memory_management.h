#ifndef _MEMORY_MANAGEMENT_H_
#define _MEMORY_MANAGEMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "process.h"

using namespace std;


/******************************************************************************
* Author: Zachary Pierson
* Class: MMU
*
* Description: This class is used to keep track of all the information a
* Memory Management Unit would need. By bundeling data and operations specific
* to the MMU, we can keep better track of the data and what functions can be
* applyed to that data.
*
******************************************************************************/
class MMU
{

public:

    MMU(vector<Process>& procs);
    ~MMU();
    void Setup(vector<Process>& procs);
    unsigned int* Config_Page_Table(unsigned int memReq, unsigned int & size, unsigned int & fragmentation);
    bool Access_Memory(Process& proc, unsigned int mem); 
    void Access_Multiple_Memory(vector<Process>& procs);
    void Terminate_Proc(Process& proc, unsigned int id);
    void Terminate_All_Procs(vector<Process>& procs);
    bool Create_Proc(Process& proc);
    void Create_Multiple_Procs(vector<Process>& procs);
    int Total_Fragmentation(vector<Process>& procs);
    bool TLB_Hit(unsigned int frame);

    void View_System_Info(vector<Process>& procs);
    void View_Process_Info(vector<Process>& procs);

    int virtualSpace;
    int physicalSpace;
    int pageSize;
    int numPages;
    int numFrames;
    int tlbSize;
    unsigned int* TLB;
    vector<unsigned int> freeFrames;
    vector<unsigned int> reference_string;

};

//Function Prototypes
void memory_management_sim();
int custom_menu_prompt();
double random_float( double min, double max);
int random_int(int min, int max);
bool prompt_proc_req(int& numProcs, int& low, int& high);
bool prompt_mem_access(int& low, int& high);

#endif
