#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/shm.h> // shared memory
#include <semaphore.h> //semaphore shit


using namespace std;

#define SHMKEY 1109 // 0x455

int mailbox_init(int , int );
void mailbox_remove(int );
bool mailbox_write(int , int , string );
string mailbox_read(int , int );

#endif