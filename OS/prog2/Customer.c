/*****************************************************************************
* Customer.c
* Author: Jennifer Kulich
* Class: CSC 458
* Due: April 9, 2020
* Please just check my pdf for the write-up
* *****************************************************************************/

#include "Bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//This function will call all of the requesting and releaseing of resources
//done by a customer
void *customer(void* customerNo){
	int custNo = *(int*)customerNo;

	while(1){
		sleep(1);
		int requestVector[NUMBER_OF_RESOURCES];

		//lock so we can do the requesting of resources
		pthread_mutex_lock(&mutex);
		for(int i = 0; i < NUMBER_OF_RESOURCES; i++){
			if(need[custNo][i] != 0)
				requestVector[i] = rand() % need[custNo][i];
			else
				requestVector[i] = 0;
		}

		printf("\nRequest P%d <%d, %d, %d>\n", custNo, requestVector[0], requestVector[1], requestVector[2]);
		request_resources(custNo, requestVector);

		pthread_mutex_unlock(&mutex);

		sleep(1);
		int releaseVector[NUMBER_OF_RESOURCES];
		//lock so we can do the requesting of resources
		pthread_mutex_lock(&mutex);
		for(int i = 0; i < NUMBER_OF_RESOURCES; i++){
			if(allocation[custNo][i] != 0)
				
				releaseVector[i] = random() % allocation[custNo][i];
			else
				releaseVector[i] = 0;
		}

		release_resources(custNo, releaseVector);
		pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char const *argv[])
{
        if(argc != NUMBER_OF_RESOURCES + 1){
                printf("number of resources not correct. \n");
                return -1;
        }

        for(int i = 0; i < NUMBER_OF_RESOURCES;i++){
                available[i] = atoi(argv[i + 1]);
        }

	//initialize max and allocation 
	for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
		for(int j = 0; j < NUMBER_OF_RESOURCES; j++){
			allocation[i][j] = 0;
			maximum[i][j] = random() % 10;
		}
	}


        //initialize need 
        for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
                for(int j = 0; j < NUMBER_OF_RESOURCES; j++){

		      need[i][j] = maximum[i][j] - allocation[i][j];
                }
        }

	printInformation();
        
	pthread_t tids[NUMBER_OF_CUSTOMERS];
        int *pid = malloc(sizeof(int) * NUMBER_OF_CUSTOMERS);

        for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
                *(pid + i) = i;
                pthread_create((tids + i), NULL, customer, (pid + i));
        }

        for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
                pthread_join(*(tids + i), NULL);
        }

        return 0;


}


