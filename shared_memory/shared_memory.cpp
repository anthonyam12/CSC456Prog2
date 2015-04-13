#include "shared_memory.h"


/***************************************************************************
 * Function: main
 *
 * Description: temporary main function for testing outside dash
 *
***************************************************************************/
int main(int argc, char const *argv[])
{
	string choice;
	string message;
	int shmid;
	bool FLAG = false;

	cin >> choice;

	if (choice == "make")
	{
		// Create 4 mailboxes of 100 KB each
		shmid = mailbox_init(4, 100);
		cout << "Shared memory id: " << shmid << endl;	
		FLAG = true;	
	}
	else
	{
		cout << "Enter shared memory id: ";
		cin >> shmid;
	}

	cout << "Enter w for write or r for read: ";
	cin >> choice;

	if (choice == "w")
	{
		cout << "Enter message: ";
		cin >> message;
		// Write to mailbox
		bool test = mailbox_write(shmid,1, message);
	}
	else if (choice == "r")
	{
		// Read from mailbox
		cout << mailbox_read(shmid,1) << endl;

	}
	else
		cout << "Invalid option" << endl;

	int bob = 1;
	while(bob != 0)
		cin >> bob;

	// Remove mailboxes
	if (FLAG)
		mailbox_remove(shmid);

	return 0;
}

/***************************************************************************
 * Function:    mailbox_init
 *
 * Description: mailbox initialization function
 *
 * Input:       num - number of mailboxes
 *              size - size of each mailbox
 *
 * Output:      shared memory address
 * 
***************************************************************************/
int mailbox_init(int num, int size)
{
	// Header block size (holds size of mailboxes + number of mailboxes)
	int header_size = (sizeof(int)*2);

	// Data block size (in kilobytes)
	int data_size = num * size * 1024;

	// Get shared memory id
	int shmid = shmget(SHMKEY, header_size + data_size, IPC_CREAT | IPC_EXCL | 0666);

	if (shmid < 0)
	{
		cout << "Error, shmget failed" << endl;
		return -1;
	}
 
	// Create header:

		// Set pointer to start of header
		int *headerp = (int*)shmat(shmid, 0, 0); 

		// Write mailbox number and size to header
		*headerp = num;
		*(headerp + 1) = size; 

		// Release shared memory
		shmdt(headerp);


	// Return shared memory id
	return(shmid);
}

/***************************************************************************
 * Function:    mailbox_remove
 *
 * Description: mailbox removal function
 *
 * Input:       shmid - shared memory id
 *
 * Output:      none
 * 
***************************************************************************/
void mailbox_remove(int shmid)
{
	// Remove mailboxes
	shmctl(shmid, IPC_RMID, 0);

}


/**************************************************************************
 * Function:    mailbox_write
 *
 * Description: write data to a mailbox
 *
 * Input:       shmid - shared memory id
 *              index - mailbox number
 *              msg   - message string
 *
 * Output:      boolean write success value
 * 
**************************************************************************/
bool mailbox_write(int shmid, int index, string msg)
{
	// Set pointer to start of header
	int *headerp = (int*)shmat(shmid, 0, 0); 

	// Get number of mailboxes from header
	int num = *headerp;

	// Get size of each mailbox from header
	int size = *(headerp + 1);

	// Get mailbox pointer
	char *boxp = (char*)headerp + sizeof(int)*2 + (size*1024)*index;

	// Error check
	if (index >= num || index < 0)
	{
		cout << "Error: Invalid mailbox index" << endl;
		return false;
	}

	// If data is larger than the mailbox, truncate message and flag user 
	if (msg.length() > ((size * 1024) - 1))
	{
		memcpy(boxp, msg.c_str(), ((size * 1024) - 1));
		boxp[((size * 1024) - 1)] = '\0';

	}
	else  // Else, write entire message to mailbox
	{
		memcpy(boxp, msg.c_str(), msg.length());

	}

	// Release shared memory
	shmdt(headerp);

	return true;

}


/**************************************************************************
 * Function:    mailbox_read
 *
 * Description: read data from a mailbox
 *
 * Input:       shmid - shared memory id
 *              index - mailbox number
 *
 * Output:      data from mailbox as a string
 * 
**************************************************************************/
string mailbox_read(int shmid, int index)
{
	// Set pointer to start of header
	int *headerp = (int*)shmat(shmid, 0, 0); 

	// Get number of mailboxes from header
	int num = *headerp;

	// Get size of each mailbox from header
	int size = *(headerp + 1);

	// Get mailbox pointer
	char *boxp = (char*)headerp + sizeof(int)*2 + (size*1024)*index;

	// Error check
	if (index >= num || index < 0)
	{
		cout << "Error: Invalid mailbox index" << endl;
		return "";
	}

	// Create message string
	string msg = string(boxp);

	// Release shared memory
	shmdt(headerp);

	return msg;

}


