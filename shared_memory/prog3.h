#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/shm.h>   // shared memory
#include <semaphore.h> //semaphore shit


using namespace std;

#define SHMKEY 1109 // 0x455
#define SHMID_FILE ".shmid"

void mailbox_init(int , int );
void mailbox_remove();
void mailbox_write(int);
void mailbox_read( int );
void mailbox_copy(int , int );
bool check_num(string);

#endif