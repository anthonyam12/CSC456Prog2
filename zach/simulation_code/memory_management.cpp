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

    printf("Please specigy the following information about the system:\n");
    printf("Number of bits in virtual address space: ");
    scanf("%d", &virtualSpace);
    printf("Number of bits in physical address space: ");
    scanf("%d", &physicalSpace);
    printf("Page size (number of bits): ");
    scanf("%d", &pageSize);

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

void MMU::Terminate_Proc(Process& proc)
{
    printf("dealocating memory\n");
    for(unsigned int i = 0; i < proc.table_size; i++)
        freeFrames.push_back( proc.table[i] );

    free(proc.table);
    proc.table = NULL;
}

void MMU::Create_Proc(Process& proc)
{
    while(proc.table == NULL)
    {
        printf("memory requirement: ");
        scanf("%d", &proc.mem_req);
        proc.table = Config_Page_Table(proc.mem_req, proc.table_size, proc.frag);
    }
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

    printf("\nLogical page   %d with offset %d Accessed\n", page, offset);
    printf("\nPhysical frame %d with offset %d Accessed\n", frame, offset);

    return true;
}

int MMU::Total_Fragmentation(vector<Process>& procs)
{
    unsigned int total = 0;

    for(unsigned int i = 0; i < procs.size(); i++)
        total += procs[i].frag;
    
    return total;
}

int custom_menu_prompt()
{
    int choice;

    printf("\tUser Defined Menu\n");
    printf("\t=================\n");
    printf("\t1. Setup/Reset System\n");
    printf("\t2. Create Process\n");
    printf("\t3. Terminate Process\n");
    printf("\t4. Access Memory\n");
    printf("\t5. View Process Information\n");
    printf("\t6. View System Information\n");
    printf("\t7. Back to Main Menu\n");
    printf("Choice: ");
    scanf("%d", &choice);

    return choice;
}

void user_defined()
{
    unsigned int temp;
    unsigned int mem;
    int option;
    bool end = false;
    Process p;
    MMU sys;  
    vector<Process> procs;

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
            case 3:     //Terminate Process
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
                sys.Terminate_Proc(procs[temp]);
                procs.erase(procs.begin() + temp);
                break;
            case 4:     //Acess Memory
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
            case 5:     //View Process Information
                sys.View_Process_Info(procs);
                break;
            case 6:     //View System Information
                sys.View_System_Info(procs);
                break;
            case 7:     //Back to Main Menu
                end = true;
                break;
            default:
                printf("Please choose one of the options above...");
                break;
        }
    }
}
