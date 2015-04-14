#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <cstdlib>
#include <sys/shm.h>   // shared memory
#include <semaphore.h> //semaphore shit


using namespace std;

#define SHMKEY 1109 // 0x455
#define SHMID_FILE ".shmid"

void mailbox_init(vector<string> & );
void mailbox_remove();
void mailbox_write(vector<string> & );
void mailbox_read(vector<string> & );
void mailbox_copy(vector<string> & );
bool check_num(string);

#endif