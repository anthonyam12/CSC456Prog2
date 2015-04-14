/******************************************************************************
 * File:       dash.cpp
 * Author:     Lisa Woody
 *
 * Class:      CSC 456
 * Instructor: Dr. Karlsson
 *
 * Program:    Homework 1, 2, and 3
 * Purpose:    Process identification program          
 *
 * Compile:    make dash
 *
 * Usage:      ./dash
 *****************************************************************************/

#include "prog1.h"
#include "prog2.h"
#include "prog3.h"
#include <iostream>
#include <cctype>
#include <string.h>
#include <vector>
#include <signal.h>


using namespace std;


void help();

int main(int argc, char const *argv[])
{
	bool FLAG = true;
	string usr_input;
	vector<string> command;

    // Start catching some signals
    signal(SIGINT, sig_callback);

    // Begin command line loop
	while(FLAG)
	{
        // Clear command vector
		command.clear();

        // Prompt
		cout << "dash> ";

        // Get and tokenize user input
		if (getline(cin, usr_input))
        {
            char* temp;
            temp = strtok((char*)usr_input.c_str()," ");
            // Put words in user input in a vector
            while(temp != NULL)
            {
                command.push_back(temp);
                temp = strtok(NULL," ");
            }
        }
        else  // Fix for ctrl-D causing an infinite loop
        {
            cout << endl;
            FLAG = false;
        }
            
 

        // If user input is not <enter> or a space, match to supported command
        if(command.size() != 0)
        {
            if ( command[0] == "exit")
            {
            	FLAG = false;
            }
            else if ( command[0] == "cmdnm")
            {
                cmdnm(command);
            }
            else if ( command[0] == "pid")
            {
                pid(command);
            }
            else if ( command[0] == "systat")
            {
                systat(command);
            }
            else if ( command[0] == "cd")
            {
                cd(command);
            }
            else if ( command[0] == "signal")
            {
                send_signal(command);
            }
            else if ( command[0] == "mboxinit")
            {
                mailbox_init(command);
            }
            else if ( command[0] == "mboxdel")
            {
                mailbox_remove();
            }
            else if ( command[0] == "mboxwrite")
            {
                mailbox_write(command);
            }
            else if ( command[0] == "mboxread")
            {
                mailbox_read(command);
            }
            else if ( command[0] == "mboxcopy")
            {
                mailbox_copy(command);
            }
            else if ( command[0] == "help")
            {
                help();
            }
            else
            {
                // Check for whitespace
                bool is_whitespace = true;
                string temp = command[0];
                for(string::const_iterator k = temp.begin(); k != temp.end(); ++k)
                {
                        if ( isspace(*k) == false )
                            is_whitespace = false;
                }
                // Print usage error if not whitespace or whitespace + string
                if (!is_whitespace || command.size() > 1)
                {
                    fork_exec(command);
                }

            }


        }
        
	}


	return 0;
}




/***************************************************************************
 * Function: help
 *
 * Description: Displays supported commands and usage information
 *
***************************************************************************/
void help()
{
    cout << "Supported commands: " << endl
         << " cmdnm <pid>        - Returns the command string that started the given process" << endl
         << " pid <string>       - Returns pids whose command contains the string" << endl
         << " cd <directory>     - Change the working directory of the process."
         << " signal <num> <pid> - Sends a signal to a process" << endl
         << " <cmd1> | <cmd2>    - Pipes output from cmd1 as input into cmd2" << endl
         << " <cmd> > <file>     - Redirects output from cmd into file" << endl
         << " <cmd> < <file>     - Uses file as input for command cmd" << endl
         << " systat             - Displays system information" << endl
         << " mboxinit <number of mailboxes> <size of mailbox in kbytes> " << endl
         << "                    - Allocates shared memory mailboxes" << endl
         << " mboxdel            - Cleans up semaphores and shared memory" << endl
         << " mboxwrite <boxnum> - Write data to a shared memory mailbox" << endl
         << " mboxread <boxnum>  - Read data from a shared memory mailbox" << endl
         << " mboxcopy <boxnum1> <boxnum2>" << endl
         << "                    - Copy the contents of one mailbox to another"
         << " exit               - Exits the program" << endl
         << " help               - Displays usage information and supported commands" << endl;
}