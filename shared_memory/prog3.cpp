#include "prog3.h"


/***************************************************************************
 * Function:    mailbox_init
 *
 * Author:      Lisa Woody
 *
 * Description: mailbox initialization function
 *
 * Input:       num - number of mailboxes
 *              size - size of each mailbox
 *
 * Output:      none
 * 
***************************************************************************/
void mailbox_init(int num, int size)
{

	// Header block size (holds size of mailboxes + number of mailboxes)
	int header_size = (sizeof(int)*2);

	// Data block size (in kilobytes)
	int data_size = num * size * 1024;

	// Get shared memory id
	int shmid = shmget(SHMKEY, header_size + data_size, IPC_CREAT | IPC_EXCL | 0666);

	if (shmid < 0)
	{
		cout << "Error: Failed to set up shared memory block "
		     << "or shared memory has already been set up" << endl;
		return;
	}
 
	// Create header:

		// Set pointer to start of header
		int *headerp = (int*)shmat(shmid, 0, 0); 

		// Write mailbox number and size to header
		*headerp = num;
		*(headerp + 1) = size; 

	// Release shared memory
	shmdt(headerp);

}

/***************************************************************************
 * Function:    mailbox_remove
 *
 * Author:      Lisa Woody
 *
 * Description: mailbox removal function
 *
 * Input:       shmid - shared memory id
 *
 * Output:      none
 * 
***************************************************************************/
void mailbox_remove()
{
	// Get shared memory id using key
	int shmid = shmget(SHMKEY, 0,0);

	// Check if shared memory block has been initialized
	if ( shmid < 0)
	{
		cout << "Error: shared memory has not been set up" << endl;
		return;
	}

	// Remove mailboxes
	shmctl(shmid, IPC_RMID, 0);

}


/**************************************************************************
 * Function:    mailbox_write
 *
 * Author:      Lisa Woody
 *
 * Description: write data to a mailbox
 *
 * Input:       shmid - shared memory id
 *              index - mailbox number
 *              msg   - message string
 *
 * Output:      none
 * 
**************************************************************************/
void mailbox_write(int index)
{

	// Get shared memory id using key
	int shmid = shmget(SHMKEY, 0,0);

	// Check if shared memory block has been initialized
	if ( shmid < 0)
	{
		cout << "Error: shared memory has not been set up" << endl;
		return;
	}

	// Get data to be written
    string msg;
    cout << "Enter data to be written: ";
    cin >> msg;

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
	}
	else
	{
		// If data is larger than the mailbox, truncate message and flag user 
		if (msg.length() > ((size * 1024) - 1))
		{
			memcpy(boxp, msg.c_str(), ((size * 1024) - 1));
			boxp[((size * 1024) - 1)] = '\0';

		}
		else  // Else, write entire message to mailbox
			memcpy(boxp, msg.c_str(), msg.length());

	}

	// Release shared memory
	shmdt(headerp);

}


/**************************************************************************
 * Function:    mailbox_read
 *
 * Author:      Lisa Woody
 *
 * Description: read data from a mailbox
 *
 * Input:       shmid - shared memory id
 *              index - mailbox number
 *
 * Output:      none
 * 
**************************************************************************/
void mailbox_read(int index)
{
	// Get shared memory id using key
	int shmid = shmget(SHMKEY, 0,0);

	// Check if shared memory block has been initialized
	if ( shmid < 0)
	{
		cout << "Error: shared memory has not been set up" << endl;
		return;
	}

	// Set pointer to start of header
	int *headerp = (int*)shmat(shmid, 0, 0); 

	// Get number of mailboxes from header
	int num = *headerp;

	// Get size of each mailbox from header
	int size = *(headerp + 1);

	
	// Error check
	if (index >= num || index < 0)
	{
		cout << "Error: Invalid mailbox index" << endl;

	}
	else
	{
		// Get mailbox pointer
		char *boxp = (char*)headerp + sizeof(int)*2 + (size*1024)*index;

		// Read message string
		cout << boxp << endl;
	}


	// Release shared memory
	shmdt(headerp);
}


/**************************************************************************
 * Function:    mailbox_copy
 *
 * Author:      Lisa Woody
 *
 * Description: copy data from one mailbox to another
 *
 * Input:       shmid - shared memory id
 *              index_from - index of mailbox whose contents will be copied
 *              index_to   - index of mailbox which will receive the copy
 *
 * Output:      none
 * 
**************************************************************************/
void mailbox_copy(int index_from, int index_to)
{
	// Get shared memory id using key
	int shmid = shmget(SHMKEY, 0,0);

	// Check if shared memory block has been initialized
	if ( shmid < 0)
	{
		cout << "Error: shared memory has not been set up" << endl;
		return;
	}

	// Set pointer to start of header
	int *headerp = (int*)shmat(shmid, 0, 0); 

	// Get number of mailboxes from header
	int num = *headerp;

	// Get size of each mailbox from header
	int size = *(headerp + 1);

	// Get mailbox pointers
	char *boxp_to = (char*)headerp + sizeof(int)*2 + (size*1024)*index_from;
	char *boxp_from = (char*)headerp + sizeof(int)*2 + (size*1024)*index_to;

	// Error check
	if (index_from >= num || index_from < 0 || 
		  index_to >= num || index_to < 0 || 
		  index_to == index_from)
	{
		cout << "Error: Invalid mailbox index" << endl;
	}
	else
	{
		// Copy data from one mailbox to the other
		strcpy(boxp_to, boxp_from);
	}


	// Release shared memory
	shmdt(headerp);

}

/**************************************************************************
 * Function:    check_num
 *
 * Author:      Lisa Woody
 *
 * Description: check if string is an integer
 *
 * Input:       num - string to be tested
 *
 * Output:      true if string is an integer, false if it is not an integer
 * 
**************************************************************************/
bool check_num(string num)
{
    for(string::const_iterator k = num.begin(); k != num.end(); ++k)
    {
        if( isdigit(*k) == false )
            return false;
    }

    return true;
}

