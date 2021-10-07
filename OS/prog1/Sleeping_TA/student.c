/**
 * General structure of a student.
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "ta.h"

void *student_loop(void *param)
{
	/* varaibles */
	srandom((unsigned)time(NULL));
	int times_through_loop = 0;
	waiting_students = 0;
	int id = (*(int*)param);

	//call the hangout because all students start as hanging out
	hang_out(id, rand() % MAX_SLEEP_TIME);

	while (times_through_loop < 5) {
		times_through_loop++;

		/* acquire the mutex lock */
		if (pthread_mutex_lock(&mutex_lock) != 0 )
			printf("StudentA %s\n",strerror(errno));

		/* is there a seat available */
		if (waiting_students < NUM_OF_SEATS ) {
			//increase number of students waiting because the student just sat down
			waiting_students++;
			printf("\t\tStudent %d takes a seat waiting = %d\n", id , waiting_students );

			//call the student semaphore
			if (sem_post(&students_sem) != 0)
				printf("StudentB %s\n",strerror(errno));
			
			student_number = id;

			//release the mutex lock since the student sat down
			if (pthread_mutex_unlock(&mutex_lock) != 0 )
				printf("StudentC %s\n",strerror(errno));

			//call the ta semaphore
			if ( sem_wait(&ta_sem) != 0)
				printf("StudentD %s\n",strerror(errno));

			printf("Student %d receiving help\n",id );
		} 
		else {
			//print out that the student cannot sit, unlock the mutex, and then
			//have the student hang out for a random time
			printf("\t\t\tStudent %d will try later\n", id );
			pthread_mutex_unlock(&mutex_lock);
			hang_out(id, rand() % MAX_SLEEP_TIME);
		}
	}
}

