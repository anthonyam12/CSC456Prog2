#include "memory_management.h"


/******************************************************************************
* Author: Zachary Pierson
* Function: MMU Constructor
*
* Description: The MMU constructor makes a call to the Setup member function.
* This basically initializes a bunch of variables and dynamically allocates 
* memory for the Transition Lookaside Buffer.
*
* param[in]  procs - vector of empty Processes
*
******************************************************************************/
MMU::MMU(vector<Process>& procs)
{
    tlbSize = -1;
    Setup(procs);
}

/******************************************************************************
* Author: Zachary Pierson
* Function: MMU Destructor
*
* Description: The MMU destructor deallocates the memory that was dynamically
* allocated to the TLB in the constructor or call to the Setup function.
*
******************************************************************************/
MMU::~MMU()
{
    free(TLB);
}

/******************************************************************************
* Author: Zachary Pierson
* Function: Setup
*
* Description: This function is called in the constructor for the MMU class.
* The reason why this is used as another function is incase the user would
* like to reset some of the system variables. The function prompts the user
* for member class variables and does error checking along the way. The 
* information that the user provides will be used to calculate the number of
* pages and frames. It is important to note that the user provides the number
* of bits for the following 3 things: virtual address space, physical address
* space, and the page size. This makes sure that the input is a power of 2.
* The TLB is also dynamically allocated and initialized with entries in this
* function.
*
* param[in/out]  procs - vector of current running processes
*
******************************************************************************/
void MMU::Setup(vector<Process>& procs)
{
    int temp;
    //need to erase all existing processes if the system changes
    procs.clear();

    printf("Please specify the following information about the system:\n");
    printf("Number of bits in virtual address space (0-32): ");
    scanf("%d", &virtualSpace);
    while( virtualSpace > 32 || virtualSpace < 0 )
    {
        printf("please specify a nuber between 0 and 32\n");
        printf("Number of bits in virtual address space (0-32): ");
        scanf("%d", &virtualSpace);
    }
    printf("Number of bits in physical address space (0-32): ");
    scanf("%d", &physicalSpace);
    while( physicalSpace > 32 || physicalSpace < 0 )
    {
        printf("please specify a nuber between 0 and 32\n");
        printf("Number of bits in physical address space (0-32): ");
        scanf("%d", &physicalSpace);
    }
    printf("Page size (number of bits 0-32): ");
    scanf("%d", &pageSize);
    while( pageSize > 32 || pageSize < 0 )
    {
        printf("please specify a nuber between 0 and 32\n");
        printf("Page size (number of bits 0-32): ");
        scanf("%d", &pageSize);
    }

    //get number of pages and frames (bits)
    numPages = virtualSpace - pageSize;
    numFrames = physicalSpace - pageSize;

    temp = pow(2.0, numFrames);

    printf("Transition Lookaside Buffer Size (1-%d): ", temp);
    scanf("%d", &tlbSize);
    while( tlbSize > temp || tlbSize <= 0 )
    {
        printf("please specify a nuber between 1 and %d\n", temp);
        printf("Transition Lookaside Buffer Size (1-%d): ", temp);
        scanf("%d", &tlbSize);
    }

    //Free TLB allocated memory if necessary
    if(tlbSize != -1)
        free(TLB);

    //create TLB and Initialize it
    TLB = (unsigned int*)malloc(tlbSize * sizeof(unsigned int));
    for(int i = 0; i < tlbSize; i++)
    {
        TLB[i] = i;
        reference_string.push_back(i);
    }

    //Print logistic information to the user
    printf("number of pages:  %u\n", (unsigned int)pow(2.0, numPages));
    printf("number of frames: %u\n", (unsigned int)pow(2.0, numFrames));

    //init freeFrames
    freeFrames.clear();
    freeFrames.resize( (int)pow(2.0, numFrames) );
    for(unsigned int i = 0; i < freeFrames.size(); i++)
        freeFrames[i] = i;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: View_System_Info
*
* Description: Prints information about the memory management Unit to the user.
*
* param[in]  procs - vector of processes used to display information
*
******************************************************************************/
void MMU::View_System_Info(vector<Process>& procs)
{
    printf("\nCurrent number of processes: %u\n", procs.size());
    printf("Total amount of internal fragmentation: %u\n", Total_Fragmentation(procs));
    printf("%u bits in virtual adress space (%u bytes)\n", virtualSpace, (unsigned int)pow(2.0, virtualSpace));
    printf("%u bits in physical adress space (%u bytes)\n", physicalSpace, (unsigned int)pow(2.0, physicalSpace));
    printf("Page Size: %u bytes, (%u bits)\n", (unsigned int)pow(2.0, pageSize), pageSize);
    printf("%u Free frames out of %u\n", freeFrames.size(), (unsigned int)pow(2.0,numFrames));
    printf("%u Possible pages\n", (unsigned int)pow(2.0,numPages));
    printf("TLB Size: %d\n", tlbSize);
}

/******************************************************************************
* Author: Zachary Pierson
* Function: View_Process_Info
*
* Description: Displays process information including the page table for each
* process and thier amount of internal fragmentation.
*
* param[in]  procs - vector of processes used to display information
*
* return
*
******************************************************************************/
void MMU::View_Process_Info(vector<Process>& procs)
{
    printf("Total internal fragmentation: %d\n", Total_Fragmentation(procs));
    for(unsigned int i = 0; i < procs.size(); i++)
    {
        if(procs[i].table_size == 0)
            continue;

        printf("Process %u requires %u bytes of memory ", i, procs[i].mem_req);
        printf("(%d bytes of internal fragmentation)\n", procs[i].frag);
        printf("\tPage | Frame  #  Page | Frame  #  Page | Frame  #");
        
        for(int j = 0; j < procs[i].table_size; j++)
        {
            if( j % 3 == 0 )
                printf("\n\t");
            printf(" %-4d|  %-5u #  ", j, procs[i].table[j]);
        }
        printf("\n");
    }
}


/******************************************************************************
* Author: Zachary Pierson
* Function: Config_Page_Table
*
* Description: This function creates a page table for a process if there are
* enough free frames available. A free frames are assigned to the process and 
* then taken out of the free frames vector. diagnostic information is printed
* to the terminal.
*
* param[in]  memReq - amount of memory the proccess is requesting
* param[out]  pageAlloc - number of pages allocated to the process
* param[out]  fragmentation - bytes of internal fragmentation 
*
* returns: Dynamically allocated page table
* returns NULL: not enough free frames were available to allocate page table
*
******************************************************************************/
unsigned int* MMU::Config_Page_Table(unsigned int memReq, unsigned int & pageAlloc, unsigned int & fragmentation)
{
    unsigned int* table;
    unsigned int size = pow(2.0, pageSize);
    double pages = memReq / (double)size;
    fragmentation = ( size - (memReq % size) ) % size;
    pageAlloc = (unsigned int)ceil(pages);

    //check if enough space to allocate
    if(freeFrames.size() < pageAlloc)
    {
        printf("Not enough free frames to allocate...\n");
        return NULL;
    }

    //allocate memory for the page table
    table = (unsigned int*) malloc( pageAlloc * sizeof(unsigned int) );

    //allocate freeFrames to the pageTable
    for(unsigned int i = 0; i < pageAlloc; i++)
    {
        table[i] = freeFrames.back();
        freeFrames.pop_back();
    }

    printf("%u pages allocated to the new process\n", pageAlloc);
    printf("There are now %u free frames\n", freeFrames.size());
    printf("internal fragmentation: %d\n", fragmentation );

    return table;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: Terminate_Proc
*
* Description: This function gives back the frames that it was allocated
* to the Memory Management Unit. The page table is then dealocated and set
* to NULL.
*
* param[in/out]  proc - Process to be terminated
* param[in]  id - id of process to be terminated. solely used for output.
*
******************************************************************************/
void MMU::Terminate_Proc(Process& proc, unsigned int id)
{
    printf("Terminating process %u\n", id);
    for(unsigned int i = 0; i < proc.table_size; i++)
        freeFrames.push_back( proc.table[i] );

    free(proc.table);
    proc.table = NULL;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: Terminate_All_Procs
*
* Description: this function makes successive calls to Terminate_Proc on all 
* processes that are currently running.
*
* param[in/out]  procs - vector of current running processes
*
*
******************************************************************************/
void MMU::Terminate_All_Procs(vector<Process>& procs)
{
    printf("Terminating %u processes\n", procs.size());
    
    for(unsigned int i = 0; i < procs.size(); i++)
        Terminate_Proc(procs[i], i);

    procs.clear();
}


/******************************************************************************
* Author: Zachary Pierson
* Function: Access_Memory
*
* Description: The process porvided attempts to access memory at location mem. 
* The page number and offset are extracted from mem and checks if the memory
* access is valid. If it is not valid, then there was a trap and return false.
* Otherwise the memory access is checked with the contents of the TLB and 
* determines if there was a TLB hit or miss.
*
* param[in]  proc - process that wants to access memory
* param[in]  mem - memory to be accessed by the given process
*
* return true: successful memory access perfomed
* return false: memory access was unsuccessful
*
******************************************************************************/
bool MMU::Access_Memory(Process& proc, unsigned int mem)
{
    unsigned int page;
    unsigned int frame;
    unsigned int offset;

    page = mem >> pageSize;             //page look up
    offset = mem - (page << pageSize);  //offset

    if(page >= proc.table_size)
    {
        printf("Trap: there is no such page %d\n", page);
        return false;
    }

    frame = proc.table[page];

    if( TLB_Hit(frame) )
        printf("TLB Hit!\n");
    else
        printf("TLB Miss. LRU algorithm used to replace page.\n");

    printf("Logical page %d (frame %d) with offset %d Accessed\n", page, frame, offset);

    return true;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: TLB_Hit
*
* Description: This function checks the TLB array to see if there is a hit.
* A real TLB is implemented with a key value pair, However we can simulate it
* with just a look up and then use the page table anyway. The TLB simulation
* implements the Least Recently Used algorithm to replace its contents.
*
* param[in] page - the page to be looked up
*
* return true: TLB hit
* return false: TLB miss
*
******************************************************************************/
bool MMU::TLB_Hit(unsigned int page)
{
    vector<unsigned int> recent;
    reference_string.push_back(page);
    bool found;
    int index;

    //if tlb hit and return true
    for(int i = 0; i < tlbSize; i++)
    {
        if( TLB[i] == page )
            return true;
    }

    //TLB miss: apply least recently used algorithm
    //look back and find least recently used page
    for(int i = reference_string.size() - 1; i >= 0 && (int)recent.size() < tlbSize; i--)
    {
        //Only push different page numbers. Check if different
        found = false;
        for(unsigned int k = 0; k < recent.size(); k++)
        {
            if(recent[k] == reference_string[i])
            {
                found = true;
                break;
            }
        }

        if(!found)
            recent.push_back(reference_string[i]);
    }
    
    //find index where least recent page accessed.
    for(index = 0; index < tlbSize; index++)
    {
        if( TLB[index] ==  recent.back() )
            break;
    }

    TLB[index] = reference_string.back();

    return false;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: Access_Multiple_Memory
*
* Description: This function makes successive calls to Access_Memory on each
* of the running processes. The function also prompts the user to define a
* range of values to be used to access memory.
*
* param[in]  procs - vector of current running processes
*
******************************************************************************/
void MMU::Access_Multiple_Memory(vector<Process>& procs)
{
    int low;
    int high;
    int mem;

    //prompt user for accessing memory.
    if( !prompt_mem_access(low, high) )
    {
        printf("No memory accessed...\n");
        return;
    }

    for(unsigned int i = 0; i < procs.size(); i++)
    {
        mem = random_int(low, high);
        printf("Process %u tries to access memory at location %d:\n", i, mem);
        Access_Memory(procs[i], mem);
    }

}

/******************************************************************************
* Author: Zachary Pierson
* Function: Total_Fragmentation
*
* Description: This funciton simply adds up all of the internal fragmentation
* from all the processes.
*
* param[in]  procs - vector of current running processes
*
* return total: total number of internal fragmentation in bytes
*
******************************************************************************/
int MMU::Total_Fragmentation(vector<Process>& procs)
{
    unsigned int total = 0;

    for(unsigned int i = 0; i < procs.size(); i++)
        total += procs[i].frag;
    
    return total;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: Create_Proc
*
* Description: Promts the user to specify how much memory the process is
* requesting. The page table for the process is then created.
*
* param[in/out]  proc - Process to be created
*
* return true: Process successfully created
* return false: Process unsuccessfully created
*
******************************************************************************/
bool MMU::Create_Proc(Process& proc)
{ 
    printf("memory requirement: ");
    scanf("%d", &proc.mem_req);
    proc.table = Config_Page_Table(proc.mem_req, proc.table_size, proc.frag);

    if(proc.table == NULL)
    {
        printf("Process not created...\n");
        return false;
    }

    return true;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: Create_Multiple_Procs
*
* Description: Prompts user for number of processes to be created along with
* upper and lower bounds for memory requirements for all of the processes.
* New processes are attempted to be created (allocate page table from free)
* frames) and pushed onto the vector of running processes. Once a process 
* fails to be created, no other porcesses created.
*
* param[in/out]  procs - vector of processes to be started
*
* return
*
******************************************************************************/
void MMU::Create_Multiple_Procs(vector<Process>& procs)
{
    Process p;
    int numProcs;
    int low;
    int high;
    int i;

    //get user input: number of processes, upper and lower bounds
    //on memory requirement
    if( !prompt_proc_req(numProcs, low, high) )
    {
        printf("No processes created\n");
        return;
    }

    //start creating processes
    for(i = 0; i < numProcs; i++)
    {
        //create process
        procs.push_back(p);
        procs.back().mem_req = random_int(low, high);
        procs.back().table = Config_Page_Table(procs.back().mem_req, procs.back().table_size, procs.back().frag);

        //check if table allocated properly
        if(procs.back().table == NULL)
        {
            printf("Process %u not created...\n", procs.size());
            procs.pop_back();
            break;
        }
    }

    printf("%d new processes created!\n", i);
}

/******************************************************************************
* Author: Zachary Pierson
* Function: prompt_mem_access
*
* Description: This function prompts the user for bounds on memory access. 
* This function also provides error checking for invalid input.
*
*
* param[in/out]  low - lower bound on memory access
* param[in/out]  high - upper bound on memory access
*
* return true: user input is valid
* return false: user input is not valid
*
******************************************************************************/
bool prompt_mem_access(int& low, int& high)
{
    //check for invalid input on lower bound
    printf("lower bound for memory access: ");
    scanf("%d", &low);
    if(low < 0)
    {
        printf("Can't access negative memory address...\n");
        return false;
    }

    //check for ivalid input on upper bound
    printf("uppoer bound for memory access: ");
    scanf("%d", &high);
    if(high < low)
    {
        printf("upper bound needs to be higher than lower bound...\n");
        return false;
    }

    return true;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: prompt_proc_req
*
* Description: This function propmts the user for information about creating
* multiple processes. Error checking is also handled in this function.
*
*
* param[in/out]  numProcs - number of processes to be created
* param[in/out]  low - lower bound for memory request
* param[in/out]  high - upper bound for memory request
*
* return true: user input is valid
* return false: user input is not valid
*
******************************************************************************/
bool prompt_proc_req(int& numProcs, int& low, int& high)
{
    //check for invalid amount of processes
    printf("Number of processes to create: ");
    scanf("%d", &numProcs);
    if(numProcs < 0)
    {
        printf("Can't create a negative number of processes...\n");
        return false;
    }

    //check for invalid input on lower bound
    printf("lower bound for memory requirement: ");
    scanf("%d", &low);
    if(low < 0)
    {
        printf("Can't allocate negative amount of sace for a process...\n");
        return false;
    }

    //check for ivalid input on upper bound
    printf("uppoer bound for memory requirement: ");
    scanf("%d", &high);
    if(high < low)
    {
        printf("upper bound needs to be higher than lower bound...\n");
        return false;
    }

    return true;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: custom_menu_prompt
*
* Description: This function displays options that the user will be able to
* choose from. The choice is returned. Error checking will have to be handled
* in the calling function.
*
* return choice: option that the user chose
*
******************************************************************************/
int custom_menu_prompt()
{
    int choice;

    printf("\tUser Defined Menu\n");
    printf("\t=================\n");
    printf("\t1.  Reset System\n");
    printf("\t2.  Create Single Process\n");
    printf("\t3.  Create Multiple Processes\n");
    printf("\t4.  Terminate Process\n");
    printf("\t5.  Terminate All Processes\n");
    printf("\t6.  Access Memory\n");
    printf("\t7.  Access Multiple Memory\n");
    printf("\t8.  View Process Information\n");
    printf("\t9.  View System Information\n");
    printf("\t10. Back to Main Menu\n");
    printf("Choice: ");
    scanf("%d", &choice);

    return choice;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: memory_management_sim
*
* Description: This is the main body of the memory management simulation.
* Options will be made available for the user to chose what to do. The 
* intent of this simulation is to get the user familiar with aspects of 
* the MMU  and how they work by trying random things. The three main 
* options are as follows:
*   1. Create Process
*   2. Terminate Proccess
*   3. Access Memory
* There are also other options to reset the variables of the MMU and view
* information about the processes and MMU.
*
******************************************************************************/
void memory_management_sim()
{
    unsigned int temp;
    unsigned int mem;
    int option;
    bool end = false;
    Process p;
    vector<Process> procs;
    MMU sys(procs);  

    //seed random number generator
    srand(time(0));

    while(!end)
    {
        option = custom_menu_prompt();

        switch(option)
        {
            case 1:     //Setup/Reset System
                sys.Setup(procs);
                break;
            case 2:     //Create Process
                procs.push_back(p);
                sys.Create_Proc(procs.back());
                break;
            case 3:     //Create number of processes
                sys.Create_Multiple_Procs(procs);
                break;
            case 4:     //Terminate Process
                if(procs.empty())
                {
                    printf("There are no current processes\n");
                    break;
                }
                sys.View_Process_Info(procs);
                printf("Process to terminate: ");
                scanf("%u", &temp);
                if(temp >= procs.size())
                {
                    printf("Invalid process id...\n");
                    break;
                }
                sys.Terminate_Proc(procs[temp], temp);
                procs.erase(procs.begin() + temp);
                break;
            case 5: //Terminate all Processes
                sys.Terminate_All_Procs(procs);
                break;
            case 6:     //Acess Memory
                if(procs.empty())
                {
                    printf("There are no current processes\n");
                    break;
                }
                sys.View_Process_Info(procs);
                printf("Process that will access memory: ");
                scanf("%u", &temp);
                if(temp >= procs.size())
                {
                    printf("Invalid process id...\n");
                    break;
                }
                printf("Memory to access: ");
                scanf("%u", &mem);
                sys.Access_Memory(procs[temp], mem);
                break;
            case 7:     //View Process Information
                sys.Access_Multiple_Memory(procs);
                break;
            case 8:     //View Process Information
                sys.View_Process_Info(procs);
                break;
            case 9:     //View System Information
                sys.View_System_Info(procs);
                break;
            case 10:     //Back to Main Menu
                end = true;
                break;
            default:
                printf("Please choose one of the options above...");
                break;
        }
    }

    sys.Terminate_All_Procs(procs);
}

/******************************************************************************
* Author: Zachary Pierson
* Function: random_float
*
* Description: This function simply grabs a random number that falls between
* the minimum and maximum bounds specifyed.
*
* param[in]  min - minimum bound for random generated number
* param[in]  max - maximum bound for random generated number
*
* return: random generated number between the specifyed bounds
*
******************************************************************************/
double random_float( double min, double max)
{
    return ((double)rand()/(double)RAND_MAX)*(max-min) + min;
}

/******************************************************************************
* Author: Zachary Pierson
* Function: random_float
*
* Description: This function simply grabs a random number that falls between
* the minimum and maximum bounds specifyed.
*
* param[in]  min - minimum bound for random generated number
* param[in]  max - maximum bound for random generated number
*
* return: random generated number between the specifyed bounds
*
******************************************************************************/
int random_int(int min, int max)
{
    return rand()%(max-min) + min + 1;
}
