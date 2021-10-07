/**
 * A solution to the pid manager problem. 
 *
 */


#include "pid.h"
#include <pthread.h>
#include <stdio.h>

// Allocates the pid map.
int allocate_map(void) 
{
	//get the number of spots in the array that will
	//be filled
	int sizeArray = PID_MAX - PID_MIN + 1;

	//initialize the entire pid_map to 0
	for(int i = 0; i < sizeArray; i++){
		pid_map[i] = 0;   
	}
	
	//set last to the pid min since this will be the 
	//first one used
	last = PID_MIN;

	return 0;
}

// Allocates a pid
int allocate_pid(void)
{
	/* local variables */
	int i;
	int sizeArray = PID_MAX - PID_MIN + 1;
	int flag = 0;

	//start the flag off as false every time- no pid has
	//been found available
	flag = 0;

	/* acquire the mutex lock and warn if unable */
	if(pthread_mutex_lock(&mutex) != 0)
		printf("unable to aquire lock\n");

	//increment the last one used
	last++;
	//check if it is greater than the max, if it is, set it back
	//to the min
	if(last > PID_MAX)
		last = PID_MIN + 1;

	/* find the next available pid */
	for(i = last - 1 - PID_MIN; i < sizeArray; i++)
	{
		//check if the pid is available in the pid map, set the pid map
		//to the last one and set the flag to show that one has been 
		//found
		if(pid_map[i] == 0)
		{
			last = 1 + i + PID_MIN;
			pid_map[i] = 1;
			flag = 1;
			break;
		}
	}


	/* release and warn if the mutex was not released */
	if(pthread_mutex_unlock(&mutex) != 0)
		printf("unable to release lock");

	//if one was found, return the pid, if not, return -1
	if(flag == 1)
		return last;
	else
		return -1;

}

// Releases a pid
void release_pid(int pid)
{
	/* acquire the mutex lock and warn if unable */
	if(pthread_mutex_lock(&mutex) != 0)
		printf("could not aquire lock\n");

	//set the pid map pid location to 0 since that pid has been
	//released and is now available
	pid_map[pid - 1- PID_MIN] = 0;

	/* release and warn if the mutex was not released  */
	if(pthread_mutex_unlock(&mutex) != 0)
		printf("unable to release lock");

	return;

}

