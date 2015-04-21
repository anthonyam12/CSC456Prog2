/******************************************************************************
* Author(s): Zachary Pierson, Anthony Morast
* CSC 456 - Operating Systems, Spring 15
* Dr. Christer Karlsson 
*
*
* This header contains all the methods required to simlulate page replacement 
* algorithms. The algorithms simulated are First-In-First-Out (FIFO), Optimal, 
* LRU, LFU, Second Chance, and Clock. Both reference and distance strings will
* be used and for bit strings focus will be given to left entry-right shift. 
*******************************************************************************/

/******************************************************************************
                                Includes 
******************************************************************************/
#include <iostream>

using namespace std;

void PrintResults(int **stack, int *reference_string, int num_accesses, int num_frames);

/******************************************************************************
                        Page Replacement Algorithms
******************************************************************************/

/******************************************************************************
* Author: Zachary Pierson
*
*
******************************************************************************/
void fifo ()
{

}

/******************************************************************************
* Author: Zachary Pierson
*
*
******************************************************************************/
void optimal ()
{

}

/******************************************************************************
* Author: Zachary Pierson
*
*
******************************************************************************/
void lru ()
{

}

/******************************************************************************
* Author: Anthony Morast
*
* This method will implement the Least Frequently used page replacement
* algorithm. The method goes through each index in the reference string and 
* determines if the page is currently loaded into memory. If it is not the and 
* there is an open space in the "stack" which contains all the frames the page
* is simply added to the "stack" and a page fault occurs. Otherwise, the least
* frequently used page is determined and is swapped for the newly needed page. 
* The frames at each index in the reference string along with the number of 
* page hits and misses is output at the end of the algorithms execution.
******************************************************************************/
void lfu (int num_accesses, int *reference_string, int num_pages, int num_frames)
{
	// will represent the stack at each time step 
	int **each_frame = new int*[num_accesses];
	for (int i = 0; i < num_accesses; i++)
		each_frame[i] = new int[num_frames];
	int *stack = new int[num_frames];

	// keep track of use frequency
	int *uses = new int[num_pages];
	for (int i = 0; i < num_pages; i++)
		uses[i] = 0;

	// initialize stack 
	for (int j = 0; j < num_frames; j++)
		stack[j] = -10;

	// some variables for display
	int page_faults = 0;
	int page_hits = 0;

	// will determine if the page is inserted in an empty frame or is in a frame
	bool added = false;
	bool contains = true;

	cout << "--- LFU Page Replacement ---" << endl;
    // apply the algorithm 
    for (int i = 0; i < num_accesses; i++)
    {
    	uses[reference_string[i]]++; // increase use frequency
    	added = false;
    	contains = false;

    	// check if page is in a frame already
    	for (int j = 0; j < num_frames; j++)
    	{
    		if (stack[j] == reference_string[i])
    		{
    			contains = true;
    			page_hits++;
    			break;
    		}
    	}

    	// if it's not already loaded up
    	if (!contains)
    	{
    		// there was a page fault
    		page_faults++;

    		// check if it is added to an empty frame
	    	for (int j = 0; j < num_frames; j++)
	    	{
	    		// add to frame if empty
	    		if (stack[j] == -10)
	    		{
	    			stack[j] = reference_string[i];
	    			added = true;
	    			break;
	    		}
	    	}
			// if the page isn't inserted into an empty frame, apply the algorithm to put it in memory
	    	if (!added) 
	    	{
	    		// find least frequently used
	    		int min = uses[stack[0]];
	    		int index = -1;
	    		for (int j = 0; j < num_frames; j++)
	    		{
	    			if (uses[stack[j]] <= min)
	    			{
	    				min = uses[stack[j]];
	    				index = j;
	    			}
	    		}
	    		// replace lfu with new page
	    		stack[index] = reference_string[i];
	    	}
    	}
    	for(int j = 0; j < num_frames; j++)
    		each_frame[i][j] = stack[j];
    }

    // print results
    PrintResults(each_frame, reference_string, num_accesses, num_frames);
    cout << "Page Faults: " << page_faults << endl;
    cout << "Page Hits: " << page_hits << endl << endl;

    delete[] uses;
    return;
}

/******************************************************************************
* Author: Anthony Morast
*
*
******************************************************************************/
void second_chance (int num_accesses, int *reference_string, int num_pages, int num_frames)
{
    
}

/******************************************************************************
* Author: Anthony Morast
*
*
******************************************************************************/
void clock_alg (int num_accesses, int *reference_string, int num_pages, int num_frames)
{
    
}

/******************************************************************************
* Author: Anthony Morast
*
* This method will print the value of the "stack" at the end of the page
* replacement algorithm. This includes outputing the values that were in the 
* frames at each index in the reference string. 
******************************************************************************/
void PrintResults(int **stack, int *reference_string, int num_accesses, int num_frames)
{
	// display the reference string
	cout << endl << "Reference String: " << endl;
    for (int i = 0; i < num_accesses; i++)
    {
    	cout << "  ";
    	cout <<  reference_string[i];
    }
    cout << endl;
    for (int i = 0; i < num_accesses; i++)
    	cout << "___";
    cout << endl;

    // display the frames
    for (int j = 0; j < num_frames; j++)
    {
    	for (int i = 0; i < num_accesses; i++)
    	{
    		if (stack[i][j] == -10)
    			cout << "|  ";
    		else
    			cout << "| " << stack[i][j];
    	}
    	cout << endl;
    }
    cout << endl;
}