 /*****************************************************************************
  * Floyds.cu
  * Author: Jennifer Kulich
  * Class: CSC 410
  * Due: Oct 19, 2020
  * 
  * This will implement Floyd's algorithm using CUDA for parallelization. Description
  * of the program and what it does is in the pdf 
  *
  *
  ****************************************************************************/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int THREAD_COUNT = 512;
int NUMBER_VERTICES = 6;
#define INF INT_MAX
int VERTICES = 6;

void printSolution(int graph[][VERTICES]);
void InnerLoop(int** graph, int k);

double floydsAlgorithm(int** graph)
{
	int dist[VERTICES][VERTICES], i, j, k;

	for(i = 0; i < VERTICES; i++)
	{
		for(j = 0; j < VERTICES; j++)
		{
			dist[i][j] = graph[i][j];
		}
	}

	//this is where I would copy the graph to the GPU

	for(k = 0; k < VERTICES; k++)
	{
		InnerLoop<<<dim3(x,y,z),dim3(a,b,c)>>>(dist, k);
		cudaDeviceSynchronize();
	}

	//This is where I would copy it back to the host

	printSolution(dist);
	return totalTime;
}

__global__ void InnerLoop(int **graph, int k)
{	
	//in here, I would find an index using the blockID, blockDimension, and threadIDx
	int t;

	for(int i = 0; i < VERTICES, i++)
	{
		for(in j = 0; j < VERTICES; j++)
		{
			t = graph[i][k] + graph[k][j];
			graph[i][j] = t * (t<graph[i][j]) + graph[i][j] * (t >= graph[i][j]);i 			
		}
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


int main(int argc, char **argv)
{
	
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
//	for(i = 0; i < VERTICES; i++)
//	{
//		for(j = 0; j < VERTICES; j++)
//		{
//			if(i == j)
//				graph[i][j] = 0;
//			else
//			{
//				if(rand() % 4 == 0)
//					graph[i][j] = (rand() % 20) + 1;
//				else
//					graph[i][j] = INF;
//			}
//
//		}
//	}
//
//	for(int i = 0; i < VERTICES; i++)
//	{
//		for(j = 0; j < VERTICES; j++)
//		{
//			if(graph[i][j] == INF)
//                                printf("%7s", "INF");
//                         else
//                                 printf("%7d", graph[i][j]);
//                 }
//                 printf("\n");

//	}

	printf("\n\n");
	graph[0][0] = 0;
	graph[0][1] = 2;
	graph[0][2] = 4;
	graph[0][3] = INF;
	graph[0][4] = INF;
	graph[0][5] = INF;

	graph[1][0] = INF; 
	graph[1][1] =0;
        graph[1][2] = 7;
        graph[1][3] = 1;
        graph[1][4] = INF;
        graph[1][5] = 8;

	graph[2][0] = INF;
        graph[2][1] = INF;
        graph[2][2] = 0;
        graph[2][3] = 4;
        graph[2][4] = INF;
        graph[2][5] = INF;

	graph[3][0] = INF;
	graph[3][1] = INF;
	graph[3][2] = INF;
	graph[3][3] = 0;
	graph[3][4] = 3;
	graph[3][5] = INF;

	graph[4][0] = INF;
	graph[4][1] = INF;
	graph[4][2] = 2;
	graph[4][3] = INF;
	graph[4][4] = 0;
	graph[4][5] = 3;

	graph[5][0] = INF;
	graph[5][1] = 5;
	graph[5][2] = INF;
	graph[5][3] = 2;
	graph[5][4] = 4;
	graph[5][5] = 0;
	

	return 0;
}
