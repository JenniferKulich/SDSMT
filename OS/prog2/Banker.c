/*****************************************************************************
* Banker.c
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

int request_resources(int customer_num, int request[]){
	if(checkIfGreaterThanNeeded(customer_num, request) == -1){
		printf("requested resources is bigger than needed.\n");
		return -1;
	}
	
	if(ifEnoughAlloc(request) == -1){
		printf("There is not enough resources for this process.\n");
		printInformation();
		return -1;
	}
	
	//pretend to allocate
	for(int i = 0; i < NUMBER_OF_RESOURCES; i++){
		need[customer_num][i] -= request[i];
		allocation[customer_num][i] += request[i];
		available[i] -= request[i];
	}


	//check if still in safe state
	if(safety_test() == 0){
		printf("Safe. Request granted\n\n");
		printInformation();
		return 0;
	}
	else{
		printf("Not safe. Request not granted.\n");
		for(int i = 0; i < NUMBER_OF_RESOURCES; i++){
			need[customer_num][i] += request[i];
			allocation[customer_num][i] -= request[i];
			available[i] += request[i];
		}
		return -1;
	}
}


int release_resources(int customer_num, int release[]){
	if(ifEnoughToRelease(customer_num, release) == -1){
		printf("The process does not own enough resources to release.\n");
		return -1;
	}

	//release the resources since safe to so
	for(int i = 0; i < NUMBER_OF_RESOURCES; i++){
		allocation[customer_num][i] -= release[i];
		need[customer_num][i] += release[i];
		available[i] += release[i];
	}

	printf("\nRelease P%d <%d, %d, %d>\n\n", customer_num, release[0], release[1], release[2]);
	printInformation();
	return 0;
}

//checks if the requested release is less than what's allocated
int ifEnoughToRelease(int customerNo, int release[]){
	for(int i = 0; i < NUMBER_OF_RESOURCES;i++){
		if(release[i] <= allocation[customerNo][i])
			continue;
		else
			return -1;
	}
	return 0;
}

//checks if the requested request is less than or equal to what's in the need
int checkIfGreaterThanNeeded(int custNo, int request[]){
	for(int i = 0; i < NUMBER_OF_RESOURCES; i++){
		if(request[i] <= need[custNo][i])
			continue;
		else
			return -1;
	}

	return 0;
}


//checks if the requested request is less than or equal to what's available
int ifEnoughAlloc(int request[]){
	for(int i = 0; i < NUMBER_OF_RESOURCES;i++){
		if(request[i] <= available[i])
			continue;
		else
			return -1;
	}
	return 0;
}


int safety_test(){
	int ifFinish[NUMBER_OF_CUSTOMERS] = {0};
	int work[NUMBER_OF_RESOURCES];
	for(int i = 0; i < NUMBER_OF_RESOURCES; i++){
		work[i] = available[i];
	}

	int k;

	//I mean, commenting would just make this look so much more worse, so I'm just going to say 
	//to look at my document
	for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
		if(ifFinish[i] == 0){
			for(int j = 0; j < NUMBER_OF_RESOURCES; j++){
				if(need[i][j] <= work[j]){
					if(j == NUMBER_OF_RESOURCES - 1)
					{
						ifFinish[i] = 1;
						for(int k = 0; k < NUMBER_OF_RESOURCES; ++k){
							work[k] += allocation[i][k];
						}
						i = -1;
						break;
					}
					else
						continue;
				}
				else
					break;
			}
		}
		else
			continue;
	}

	for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
		if(ifFinish[i] == 0)
			return -1;
		else
			continue;
	}
	return 0;

}


//will format and print all the customers, their allocation, need, and what's available
void printInformation(){
	int allocationNumber;
	int needNumber;
	int availableNumber;

	printf("\tAllocation\tNeed\tAvailable\n");
	printf("\tA B C\t\tA B C\tA B C\n");
	for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
		printf("P%d\t", i);
		printf("%d %d %d\t\t%d %d %d", allocation[i][0], allocation[i][1], allocation[i][2], need[i][0], need[i][1], need[i][2]);
		
		if(i == 0){
			printf("\t%d %d %d\n", available[0], available[1], available[2]);
		}
		else
			printf("\n");
	}
}
