#include "prog1.h"

/***************************************************************************
 * Function: cmdnm
 *
 * Description: Displays the command string (name) that started the process
 *              for a user-specified process ID
 *
***************************************************************************/
void cmdnm(vector<string> & command)
{
    if (command.size() > 1)
    {
        string process = "/proc/" + command[1] + "/cmdline";
        ifstream fin(process.c_str());
        
        if (fin.is_open())
        {
            string pline;
            getline(fin,pline);
            cout << pline << endl;                  

        }
        else
        {
            cout << "Invalid pid number" << endl;
        }

    }
    else
        cout << "Usage: cmdnm <pid>" << endl;

    return;
}

/***************************************************************************
 * Function: pid
 *
 * Description: Displays all process IDs whose command strings contain the  
 *              user-specified string.
 *
***************************************************************************/
void pid(vector<string> & command)
{
    bool pid_found = false;

    if (command.size() > 1)
    {
        char dir_name[] = "/proc";
        DIR *dir = NULL;
        struct dirent *proc_entry = NULL;

        dir = opendir(dir_name);

        if(dir)
        {

            while( (proc_entry = readdir(dir)) )
            {
                if( (proc_entry -> d_type) == DT_DIR )
                {
                    bool is_Proc_Number = true;
                    string p_temp = proc_entry -> d_name;
                    for(string::const_iterator k = p_temp.begin(); k != p_temp.end(); ++k)
                    {
                        if ( isdigit(*k) == false )
                            is_Proc_Number = false;
                    }

                    if (is_Proc_Number)
                    {

                        string temp = "/proc/" + p_temp + "/cmdline";

                        ifstream fin(temp.c_str());
                        if (fin.is_open())
                        {
                            string p;
                            getline(fin, p);

                            for(string::iterator k = p.begin(); k != p.end(); ++k)
                            {
                                if (*k == '\0')
                                    *k = ' ';
                            }

                            if (strstr(p.c_str(), command[1].c_str()) != NULL)
                            {
                                cout << proc_entry -> d_name << endl;
                                pid_found = true;
                            }

                        }

                    }

                }
            }
            closedir(dir);
        }
        else
        {
            cout << "Error, cannot open directory";
        }

    }
    // Error because no pid command was entered
    else
        cout << "Usage: pid <command>" << endl;

    // Error if no such process number was discovered
    if (!pid_found)
        cout << "No matching pid found" << endl;

    return;


}

/***************************************************************************
 * Function: pid
 *
 * Description: Displays process information: Linux system version, 
 *              system uptime, memory usage information, and CPU information
 *
***************************************************************************/
void systat(vector<string> & command)
{
    ifstream fin("/proc/version");
    if (fin.is_open())
    {
        string vline;
        getline(fin,vline);
        cout << "Version: " << vline << endl;
    }
    fin.close();

    fin.open("/proc/uptime");
    if (fin.is_open())
    {
        string tline;
        getline(fin,tline);
        cout << "Uptime: " << tline << " seconds." << endl;
    }
    fin.close();

    fin.open("/proc/meminfo");
    if (fin.is_open())
    {
        string mline;
        getline(fin,mline);
        cout << mline << endl;
    }
    fin.close();

    fin.open("/proc/cpuinfo");
    if (fin.is_open())
    {
        string cline;
        for (int i = 0; i < 9; ++i)
        {
            getline(fin,cline);
            cout << cline << endl;
        }

    }
    fin.close();

    return;
}