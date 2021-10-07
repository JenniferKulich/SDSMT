 /*****************************************************************************
  * Floyds.c
  * Author: Jennifer Kulich
  * Class: CSC 410
  * Due: Oct 19, 2020
  * 
  * This implements Floyd's algorith using OpenMp for parallelization. Floyd's
  * algorithm finds the shorest path between nodes. The program will print off
  * the original random graph and then then the final graph after having gone 
  * through Floyd's algorithm. More description in the writeup.
  *
  * To compile: make clean; make all     then ->    ./floyds {number} 
  * where number is the number of vertices you would like in the graph
  *
  ****************************************************************************/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int THREAD_COUNT = 8;
int NUMBER_VERTICES = 6;
#define INF INT_MAX
int VERTICES = 6;

void printSolution(int graph[][VERTICES]);
void printDataOut(double parallelTime, double totalTime);

double floydsAlgorithm(int** graph)
{
	int dist[VERTICES][VERTICES], i, j, k;
	double t0, t1, totalTime;

	for(i = 0; i < VERTICES; i++)
	{
		for(j = 0; j < VERTICES; j++)
		{
			dist[i][j] = graph[i][j];
		}
	}

	t0 = omp_get_wtime();
#pragma omp parallel for num_threads(THREAD_COUNT)default(none)shared(dist,VERTICES)private(k,i,j)
	for(k = 0; k < VERTICES; k++)
	{
		for(i = 0; i < VERTICES; i++)
		{
			for(j = 0; j < VERTICES; j++)
			{
				if(dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j])
					dist[i][j] = dist[i][k] + dist[k][j];
			}
		}
	}
	t1 = omp_get_wtime();

	printSolution(dist);
	totalTime = t1 - t0;
	printf("Elapsed time = %f seconds on %d threads.\n", totalTime, THREAD_COUNT);
	return totalTime;
}

void printSolution(int graph[][VERTICES])
{
	for(int i = 0; i < VERTICES; i++)
	{
		for(int j = 0; j < VERTICES; j++)
		{
			if(graph[i][j] == INF)
				printf("%7s", "INF");
			else
				printf("%7d", graph[i][j]);
		}
		printf("\n");
	}
}

void printDataOut(double parallelTime, double totalTime)
{
	double speedup, efficiency;
	double sequentialPortion = totalTime - parallelTime;
	
	speedup = totalTime / (sequentialPortion + (parallelTime / THREAD_COUNT));

	efficiency = totalTime / ((sequentialPortion * THREAD_COUNT) + parallelTime);

	printf("Speedup on %d threads: %f.\n", THREAD_COUNT, speedup);
	printf("Efficiency on %d threads: %f.\n", THREAD_COUNT, efficiency);
}

int main(int argc, char **argv)
{
	double t0, t1, totalTime;
	t0 = omp_get_wtime();
	if(argc != 2)
	{
		printf("Wrong number of arguments\n");
		return -1;
	}

	VERTICES = strtol(argv[1], NULL, 10);

	int **graph;
	int i,j;
	time_t t;

	graph = malloc(VERTICES*sizeof(int *));
	if(graph == NULL)
		return -1;
	for(i = 0; i < VERTICES; i++)
	{
		graph[i] = malloc(VERTICES*sizeof(int));
		if(graph[i] == NULL)
			return -1;
	}

	srand((unsigned) time(&t));

	//randonly assign graph
	for(i = 0; i < VERTICES; i++)
	{
		for(j = 0; j < VERTICES; j++)
		{
			if(i == j)
				graph[i][j] = 0;
			else
			{
				if(rand() % 4 == 0)
					graph[i][j] = (rand() % 20) + 1;
				else
					graph[i][j] = INF;
			}

		}
	}

	for(int i = 0; i < VERTICES; i++)
	{
		for(j = 0; j < VERTICES; j++)
		{
			if(graph[i][j] == INF)
                                 printf("%7s", "INF");
                         else
                                 printf("%7d", graph[i][j]);
                 }
                 printf("\n");

	}

	printf("\n\n");

	double parallelTime = floydsAlgorithm(graph);
	t1 = omp_get_wtime();
	totalTime = t1 - t0;
	printf("Total time for whole program: %f seconds.\n", totalTime);
	printDataOut(parallelTime, totalTime);
	return 0;
}

