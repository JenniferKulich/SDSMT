#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_TICKETS 35
#define NUM_SELLERS 4
/**
 * The ticket counter and its associated lock will be accessed
 * all threads, so made global for easy access. */
static int numTickets = NUM_TICKETS;
sem_t ticketsLock;
//  The ticket selling function
void* SellTickets(void *input)
{
	bool done=false;
	int numSoldByThisThread = 0;
    //  As long as there are tickets sell them
	while(!done){
        usleep(rand()%500+1);
        // Aquire the lock
		sem_wait(&ticketsLock);
		// Critical section
		if( numTickets == 0)
		    done = true;
		else {
			numTickets--;
			numSoldByThisThread++;
			printf("Seller #%ld sold one (%d left)\n", (long)input, numTickets);
		}
		// Release the lock
		sem_post(&ticketsLock);
	}
	// No more tickets!
    printf("Seller #%ld noticed all tickets sold! (I sold %d myself) \n", (long)input, numSoldByThisThread);
    return 0;
}

int main()
{
	long i=0;
	int error;
	pthread_t tid[NUM_SELLERS];
    // Inits
	sem_init(&ticketsLock, 0, 1);
    srand(0);
    // Start the threads
	for(i=0; i<NUM_SELLERS; i++) {
		error = pthread_create(&(tid[i]), NULL, SellTickets, (void *)i);
		if (error != 0)
		    printf("\nThread can't be created : [%s]", strerror(error));
    }
    // Join the threads
	for (i = 0; i < NUM_SELLERS; i++)
	    pthread_join(tid[i], NULL);
    // Remove the lock
    sem_destroy(&ticketsLock);
    printf("All done!\n");
    return 0;
}