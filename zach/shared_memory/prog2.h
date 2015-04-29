#ifndef PROG2_H
#define PROG2_H

#include <iostream>
#include <vector>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h>
#include <cstdlib>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>
#include <signal.h>

using namespace std;

void fork_exec(vector<string> & );
void cd(vector<string> & );
void redirect(vector<string> & , int );
void pipe(vector<string> & , int );
int command_type(vector<string> & , bool & , bool & );
void execute(vector<string> & , int , int );
void send_signal(vector<string> & );
void sig_callback(int );

#endif