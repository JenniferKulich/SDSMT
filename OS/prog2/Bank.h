/*****************************************************************************
* Bamk.h
* Author: Jennifer Kulich
* Class: CSC 458
* Due: April 9, 2020
* Please just check my pdf for the write-up
* *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
pthread_mutex_t mutex;


/* these may be any values >= 0 */
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];


//functions
void *customer(void* customerNo);

int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);

int ifEnoughToRelease(int customerNo, int release[]);
int checkIfGreaterThanNeeded(int custNo, int request[]);
int ifEnoughAlloc(int request[]);
int safety_test();
void printInformation();
