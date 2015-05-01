#include "memory_management.h"

MMU::MMU()
{
    initialized = false;
}

void hello()
{
    printf("Hello World!");
}


void MMU::Setup(vector<Process>& procs)
{
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

    //Print logistic information to the user
    numPages = virtualSpace - pageSize;
    numFrames = physicalSpace - pageSize;
    printf("number of pages:  %u\n", (unsigned int)pow(2.0, numPages));
    printf("number of frames: %u\n", (unsigned int)pow(2.0, numFrames));

    //init freeFrames
    freeFrames.clear();
    freeFrames.resize( (int)pow(2.0, numFrames) );
    for(unsigned int i = 0; i < freeFrames.size(); i++)
        freeFrames[i] = i;

    initialized = true;
}

bool MMU::Is_Init()
{
    return initialized;
}

void MMU::View_System_Info(vector<Process>& procs)
{
    printf("\nCurrent number of processes: %u\n", procs.size());
    printf("Total amount of internal fragmentation: %u\n", Total_Fragmentation(procs));
    printf("%u bits in virtual adress space (%u bytes)\n", virtualSpace, (unsigned int)pow(2.0, virtualSpace));
    printf("%u bits in physical adress space (%u bytes)\n", physicalSpace, (unsigned int)pow(2.0, physicalSpace));
    printf("Page Size: %u bytes, (%u bits)\n", (unsigned int)pow(2.0, pageSize), pageSize);
    printf("%u Free frames out of %u\n", freeFrames.size(), (unsigned int)pow(2.0,numFrames));
    printf("%u Possible pages\n", (unsigned int)pow(2.0,numPages));
}

//make this a vector of processes
void MMU::View_Process_Info(vector<Process>& procs)
{
    for(unsigned int i = 0; i < procs.size(); i++)
    {
        if(procs[i].table_size == 0)
            continue;

        printf("Process %u requires %u bytes of memory\n", i, procs[i].mem_req);
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

void MMU::Terminate_Proc(Process& proc, unsigned int id)
{
    printf("Terminating process %u\n", id);
    for(unsigned int i = 0; i < proc.table_size; i++)
        freeFrames.push_back( proc.table[i] );

    free(proc.table);
    proc.table = NULL;
}

void MMU::Terminate_All_Procs(vector<Process>& procs)
{
    printf("Terminating %u processes\n", procs.size());
    
    for(unsigned int i = 0; i < procs.size(); i++)
        Terminate_Proc(procs[i], i);

    procs.clear();
}


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

    printf("Logical page %d (frame %d) with offset %d Accessed\n", page, frame, offset);

    return true;
}

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

int MMU::Total_Fragmentation(vector<Process>& procs)
{
    unsigned int total = 0;

    for(unsigned int i = 0; i < procs.size(); i++)
        total += procs[i].frag;
    
    return total;
}

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

void memory_management_sim()
{
    unsigned int temp;
    unsigned int mem;
    int option;
    bool end = false;
    Process p;
    MMU sys;  
    vector<Process> procs;

    //seed random number generator
    srand(time(0));

    //First things first: set up the system
    sys.Setup(procs);

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

double random_float( double min, double max)
{
    return ((double)rand()/(double)RAND_MAX)*(max-min) + min;
}

int random_int(int min, int max)
{
    return rand()%(max-min) + min + 1;
}
