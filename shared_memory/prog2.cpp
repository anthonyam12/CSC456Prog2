#include "prog2.h"


/***************************************************************************
 * Function: fork_exec
 *
 * Description: accepts non-shell-intrinsic commands and executes them by
 *              forking off a child process and running the command in the 
 *              child process via an exec. 
 *
***************************************************************************/
void fork_exec(vector<string> & command)
{
	int   waitpid; 
	int   childpid; 
	int   status;
	struct rusage use;
	bool isRedirect = false, isPipe = false;
	int loc;

    // Check command string for pipe or redirect symbols, return index
    loc = command_type(command, isRedirect, isPipe);


    // Make a child
	childpid = fork();

	// If child 
	if (childpid == 0) 
	{ 
		cout << "Starting child process " << getpid() << "..." << endl;
		if (isRedirect)
			redirect(command, loc);
		else if ( isPipe )
			pipe(command, loc);
		else
			execute(command, 0, command.size());
		
	}

	// Print process information for child processes
    getrusage(RUSAGE_CHILDREN, &use);

	// Wait for the child to exit.
	waitpid = wait(&status); 

    cout << "------------------------------------------" << endl;    
    cout << "Child process information:" << endl
    	 << "      Process id     : " << waitpid << endl
         << "      Exit status    : " << status << endl
         << "      User CPU Time  : " << use.ru_utime.tv_sec << "." << use.ru_utime.tv_usec << endl
         << "      System CPU Time: " << use.ru_stime.tv_sec << "." << use.ru_stime.tv_usec << endl
         << "      Num page faults: " << use.ru_majflt << endl
         << "      Num of swaps   : " << use.ru_nswap << endl;
    cout <<"------------------------------------------" << endl;

	
	return;


}

/***************************************************************************
 * Function: command_type
 *
 * Description: Function determines if user input contains redirect or pipe
 *              commands.
 *
***************************************************************************/
int command_type(vector<string> &command, bool &isRedirect, bool &isPipe)
{
	int loc = -1;
	 
    for (unsigned int j = 0;  j < command.size();  ++j)     
    {
    	
    	if (command[j] == ">" || command[j] == "<")
    	{
    		isRedirect = true;
    		loc = j;
    		j = command.size();
    	}
    	else if (command[j] == "|")
    	{
    		isPipe = true;
    		loc = j;
    		j = command.size();
    	}

    }

    return loc;
}

/***************************************************************************
 * Function: execute
 *
 * Description: calls exec() on command given in command vector
 *
***************************************************************************/
void execute(vector<string> & command, int start, int end)
{
	vector<string> temp(&command[start],&command[end]);
	const char **argv = new const char* [temp.size()+1]; 

    for (unsigned int j = 0;  j < temp.size();  ++j)     
    {
    	argv [j] = temp[j].c_str();
    }

    argv [temp.size()] = NULL;
    execvp (argv[0], (char**)argv);
	perror("Command execution failed");
	exit(1);
}

/***************************************************************************
 * Function: redirect
 *
 * Description: Implement redirection from a process to a file or vice versa
 *
 * Note: function adapted from the following code:
 		 http://www.mcs.sdsmt.edu/ckarlsso/csc456/spring15/code/jchen.c
 *
***************************************************************************/
void redirect(vector<string> & command, int loc)
{
    
    int file = -1;

    // Error checking
    if( (command.size() < 3))
    {
        cout << "Usage: <Unix command> > file " << endl 
             << "       OR" << endl
			 << "       cd <relative path>" << endl;

	    return;
    }

    //  [<] - Input from file
    if (command[loc] == "<")
    {
        if ((file = open(command[loc + 1].c_str(), O_RDONLY, S_IWUSR|S_IRUSR)) == -1)
        {
        	cout << "Unable to open " << command[loc + 1].c_str() << " for reading" << endl;
        	exit(-1);
        }

        close(0);     // close child standard input
        dup(file);    // redirect the child input
        close(file);  // close unnecessary file descriptor
        
    } //  [>] - Output to file
    else
    {
        if ((file = open(command[loc + 1].c_str(), O_CREAT|O_WRONLY, S_IWUSR|S_IRUSR)) == -1 )
        {
        	cout << "Unable to open " << command[loc + 1].c_str() << " for writing" << endl;
        	exit(-1);
        }
        
        close(1);     // close child standard output
        dup(file);    // redirect the child output
        close(file);  // close unnecessary file descriptor
    }

    // Execute left hand side (commands)
    execute(command, 0, loc);

  
}

/***************************************************************************
 * Function: pipe
 *
 * Description: implement pipe between two processes
 *
 * Note: function adapted from the following code:
 		 http://www.mcs.sdsmt.edu/ckarlsso/csc456/spring15/code/pipe1.c
 *
***************************************************************************/
void pipe(vector<string> & command, int loc)
{
    int mPipe[2];
    int pid;
    int status;

    // Error checking
    if(command.size() < 3)
    {
        return;
    }


    // Create pipe
    pipe(mPipe);

    // Create grandchild
    pid = fork();

    // Grandchild will executes output side of pipe 
    if ( pid == 0 )
    {     
        close(1);
        dup(mPipe[1]);
        close(mPipe[0]);
        close(mPipe[1]);

        // Execute left side:
        execute(command,0,loc);
        exit(1);
    }

    // Child will now execute input side of pipe
    close(0);
    dup(mPipe[0]);
    close(mPipe[0]);
    close(mPipe[1]);

    // // Wait for grandchild
    // wait(&status);

    // Execute input side:
    execute (command, (loc + 1), command.size());
    
}



/***************************************************************************
 * Function: cd
 *
 * Description: changes the current working directory
 *
***************************************************************************/
void cd(vector<string> & command)
{
    // Error checking
    if (command.size() < 2)
    {
        cout << "Usage: cd <absolute path>" << endl 
             << "       OR" << endl
			 << "       cd <relative path>" << endl;
    }
    else
    	if (chdir(command[1].c_str()) == -1)
    		cout << "Cannot access " << command[1].c_str() 
    	         << ": No such file or directory." << endl;

    return;
}


/***************************************************************************
 * Function: send_signal
 *
 * Description: sends a signal to a process
 *
***************************************************************************/
void send_signal(vector<string> & command)
{

	// Error checking
    if( command.size() != 3 )
    {
        cout << "Usage: signal <signal_num> <pid>" << endl;

	    return;
    }

	// kill(pid, signal_number);
	kill(atoi(command[2].c_str()),atoi(command[1].c_str()));
}


/***************************************************************************
 * Function: sig_callback
 *
 * Description: callback function that reports caught signals
 *
***************************************************************************/
void sig_callback(int sig)
{
	cout << endl << "Caught signal: " << sig << endl;
	if (sig == 2)
		exit(0);
}
