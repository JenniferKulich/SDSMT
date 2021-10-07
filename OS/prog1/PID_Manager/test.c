/*****************************************************************************
 * test.c
 * Author: Jennifer Kulich
 * Class: CSC 458
 * Due: March 6, 2020
 * Test the implementation of the PID manager. This will go through and 
 * allocate the use of a pid based off of the min and max range of the pids.
 * It will go through the array of pids, and if the spot is open, allocate
 * a pid there and sleep it for a random amount of time. Once that time is up,
 * the thread is released and the pid is freed. However, that does not mean
 * that the next allocated pid will be the pid of the last on released. It 
 * will keep incrementing the pid to the next available one until it hits
 * the max number the pid can be. Once it hits that max number, it will 
 * start again at the min, search for the lowest available pid and 
 * continue on.
 * This makes use of mutexes- used to access the pid map
 *
 ****************************************************************************/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "pid.h"

#define NUM_THREADS 5
#define ITERATIONS   10
#define SLEEP         5

int in_use[PID_MAX + 1];

/**
 * mutex lock used when accessing data structure
 * to ensure there are no duplicate pid's in use.
 */
pthread_mutex_t test_mutex;

void *allocator(void *param)
{
	int i, pid;

	for (i = 0; i < ITERATIONS; i++) {
		/* sleep for a random period of time */
		sleep((int)(random() % SLEEP));

		/* allocate a pid */
		pid = allocate_pid();

		//check to see if there was a pid available
		if (pid == -1)
			printf("No pid available\n");

		//if the pid was available, print it out
		else {
			/* indicate in the in_use map the pid is in use */
			printf("allocated %d\n", pid);

			//check to make sure it was not a duplicated assigned pid
			//if it is, you know something with your program is wrong
			if(in_use[pid] == 1)
				printf("duplicate");
			else
				in_use[pid] = 1;
			
			/* sleep for a random period of time */
			sleep((int) (random() % SLEEP));

			/* release the pid */
			release_pid(pid);
			printf("released %d\n", pid);
			
			//say that the pid is not in use anymore
			in_use[pid] = 0;
		}
	}
}

int main(void)
{
	int i;
	pthread_t tids[NUM_THREADS];
	num_duplicates = 0;

	for (i = 0; i <= PID_MAX; i++) {
		in_use[i] = 0;
	}

	/* allocate the pid map */
	if (allocate_map() == -1)
		return -1;

	srandom((unsigned)time(NULL));


	/* create the threads */
	for(int i = 0; i < NUM_THREADS; i++)
	{
		if(pthread_create(&tids[i], NULL, &allocator, NULL) !=0)
			return -1;
	}


	/* join the threads */
	for(int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(tids[i], NULL);
	}

	/* test is finished */
	return 0;
}
