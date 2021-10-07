/*****************************************************************************
 * Author: Jennifer Kulich
 * Class: CSC 458
 * Due Date: March 6th, 2020
 * This program is supposed to simulate students sitting outside of a TA's
 * office. There will be a certain number of students and a certain number
 * of chairs outside of the office. All students start off as hanging out 
 * working on their program. One or more of them will suddenly have a 
 * question for their TA and go to his office. If no one is there, they will
 * just go into the office. If someone is currently being helped, they will
 * attempt to take a seat. If all chairs are filled with students, they 
 * will go hangout and try again later.
 * This program utilizes semaphore and mutexes. 
 ****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include "ta.h"

pthread_t ta;
pthread_t students[NUM_OF_STUDENTS];

/**
 * Initialize all relevant data structures and
 * synchronization objects.
 */
void init()
{
	//initialize mutex lock
	if (pthread_mutex_init(&mutex_lock, NULL) != 0  )
		printf("%s\n",strerror(errno));

	//initialize student semaphore
	if (sem_init(&students_sem, 0, 0) != 0 )
		printf("error init students_sem\n"); 

	//initialize ta semaphore
	if (sem_init(&ta_sem, 0 ,0) != 0  )
		printf("error init ta\n");

	//go through all of the students and assign their id's
	for (int i = 0; i < NUM_OF_STUDENTS; i++)
		student_id[i] = i;
}

//Go through all of the studetns and create a thread for them
void create_students()
{
	for (int i = 0; i < NUM_OF_STUDENTS; i++) {
		pthread_create(&students[i], 0, student_loop, (void *)&student_id[i]);
	}
}

//Create a thread for the TA
void create_ta()
{
	pthread_create(&ta, 0, ta_loop, 0);
}

//Main: where everything is called to be initialized and then run
int main(void)
{
	int i;

	init();
	create_ta();
	create_students();

	//join all of the threads once the program is done
	for (i = 0; i < NUM_OF_STUDENTS; i++)
		pthread_join(students[i], NULL);

	/* when all students have finished, we will cancel the TA thread */	
	if (pthread_cancel(ta) != 0)
		printf("%s\n",strerror(errno));

	return 0;
}

