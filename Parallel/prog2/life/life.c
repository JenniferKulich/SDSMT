 /*****************************************************************************
  * Life.c
  * Author: Jennifer Kulich
  * Class: CSC 410
  * Due: Nov 16, 2020
  * 
  * This will simulate the 'game of life' by creating a random graph with 'live'
  * cells. The graph will simultaneouly be updated based off of the rules
  *
  * To compile: make clean; make all     
  * then ->    mpiexec -np {# processes} ./life {numberLiveCells) {iterations}
  *                      {when to print} {Rows} {Columns}
  *
  ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
int ROW = 0;
int COL = 0;
#define SEED 35791246

void printSolution(int **graph);
int **createRandomMatrix(int **graph, int numberLiveCells);
void freeGraph(int **graph);
int **getNewGraph(int **graph, int *row, int rank);
int *evaluateGraph(int *row, int *updatedRow, int *aboveRow, int *belowRow);

int main(int argc, char *argv[])
{
        int nprocs, rank;

        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int iNumberLiveCells = atoi(argv[1]);
        int jIterations = atoi(argv[2]);
        int kthToPrint = atoi(argv[3]);
        ROW  = atoi(argv[4]);
        COL  = atoi(argv[5]);

        int **graph;

        int *row = malloc(sizeof(int) * COL);
        int *aboveRow = malloc(sizeof(int) * COL);
        int *belowRow = malloc(sizeof(int) *COL);
        int *updatedRow = malloc(sizeof(int) * COL);

        MPI_Barrier(MPI_COMM_WORLD);


        if(rank == 0)
        {
                //initialize graph
                graph = (int **)malloc(sizeof(int *) * ROW);

                for(int i = 0; i < ROW; i++)
                {
                        graph[i] = (int *)malloc(sizeof(int) * COL);
                }
		//create random graph
                graph = createRandomMatrix(graph, iNumberLiveCells);

                //since this is the beginning, set the 'current' row to the first row in the graph
                for(int j = 0; j < COL; j++)
                {
                        row[j] = graph[0][j];
                }

                //send every row(but row 0) in the graph to a different process
                for(int n = 1; n < ROW; n++)
                {
                        MPI_Send(graph[n], COL, MPI_INT, n, 0, MPI_COMM_WORLD);
                }
        }
        else
        {
                //get the specific row sent from process 0
                MPI_Recv(row, COL, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }


        graph = getNewGraph(graph, row, rank);

        MPI_Barrier(MPI_COMM_WORLD);

        //if this is the first process, print the initial graph
        if(rank == 0)
        {
                printf("Initial Graph:\n");
                printSolution(graph);
        }

        MPI_Barrier(MPI_COMM_WORLD);

        //loop through the 'game' as many iterations as the user put in
        for(int iteration = 0; iteration < jIterations; iteration++)
        {
                //reset all rows to 0
                for(int i = 0; i < COL; i++)
                {
                        aboveRow[i] = 0;
                        belowRow[i] = 0;
                        updatedRow[i] = 0;
                }
                //if on top row, can only evaluate with row below it
                if(rank == 0)
                {
                        MPI_Send(row, COL, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                        MPI_Recv(belowRow, COL, MPI_INT, rank + 1, 1, MPI_COMM_WORLD, &status);
                }
                else if(rank == ROW - 1) //if on bottom row, can only evaluate with row above it
                {
                        MPI_Send(row, COL, MPI_INT, rank - 1, 1, MPI_COMM_WORLD);
                        MPI_Recv(aboveRow, COL, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
                }
		else //any other rows- need above and below row it
                {
                        MPI_Sendrecv(row, COL, MPI_INT, rank - 1, 1, belowRow, COL, MPI_INT, rank + 1, 1, MPI_COMM_WORLD, &status);
                        MPI_Sendrecv(row, COL, MPI_INT, rank + 1, 1, aboveRow, COL, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
                }

                //evaluate the graph based off of the game rules        
                updatedRow = evaluateGraph(row, updatedRow, aboveRow, belowRow);

                //set row's to their 'updated' row based on what happened in the rule eval
                for(int i = 0; i < COL; i++)
                {
                        row[i] = updatedRow[i];
                }

                MPI_Barrier(MPI_COMM_WORLD);

                //get the new graph based on the update
                graph = getNewGraph(graph, row, rank);

                MPI_Barrier(MPI_COMM_WORLD);

                //if we are on the iteration that should be printing a graph, print the graph
                if(rank == 0 && (iteration + 1) % kthToPrint == 0)
                {
                        printSolution(graph);
                }

                MPI_Barrier(MPI_COMM_WORLD);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        //free arrays
        //I have absolutely no idea why it doesn't like freeing my arrays ..... Like no idea whatsoever
//        freeGraph(graph);
//        free(aboveRow);
//        free(belowRow);
//        free(updatedRow);

        MPI_Finalize();
        return 0;
}

int **createRandomMatrix(int **graph,int numberLiveCells)
{
        int currentNumberLiveCells = 0;
        int i, j;
        srand(SEED);

        //set entire graph to be all 0's        
        for(int i = 0; i < ROW; i++)
        {
                for(int j = 0; j < COL; j++)
                {
                        graph[i][j] = 0;
                }
        }

        //loop through until there are the correct number of alive cells 
        while(currentNumberLiveCells < numberLiveCells)
        {
                for(i = 0; i < ROW; i++)
                {
                        for(j = 0; j < COL; j++)
                        {
                                if(currentNumberLiveCells < numberLiveCells)
                                {
                                        if(graph[i][j] != 1)
                                        {
                                                if(rand() % 2 == 0)
                                                {
                                                        graph[i][j] = 1;
                                                        currentNumberLiveCells++;
                                                }
                                                else
                                                        graph[i][j] = 0;
                                        }
                                }
                        }
                }
        }

        return graph;
}

int **getNewGraph(int **graph, int *row, int rank)
{
        MPI_Status status;

        if(rank == 0)
        {
                for(int i = 1; i < ROW; i++)
                {
                        MPI_Recv(graph[i], COL, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                }

                graph[0] = row;
        }
        else
        {
                MPI_Send(row, COL, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

        return graph;
}
int *evaluateGraph(int *row, int *updatedRow, int *aboveRow, int *belowRow)
{
        int numberLiveCells;
        for(int j = 0; j < COL; j++)
        {
                numberLiveCells = 0;
               for(int k = j - 1; k <= j + 1; k++)
               {
                       numberLiveCells += aboveRow[k];
                       numberLiveCells += belowRow[k];
                       //if on current row, only add cell if not itself
                       if(k != j)
                       {
                               numberLiveCells += row[k];
                       }
               }

               //if cell dead and has 3 neighbors, bring back to life
               //if cell alive and has less than 2 or more than 3 neighbors, it dies
               //otherwise, stays same
               if(row[j] == 0 && numberLiveCells == 3)
               {
                       updatedRow[j] = 1;
               }
               else if(row[j] == 1 && (numberLiveCells < 2 || numberLiveCells > 3))
               {
                       updatedRow[j] = 0;
               }
               else
               {
                       updatedRow[j] = row[j];
               }
       }

        return updatedRow;
}


void printSolution(int **graph)
{
        int i,j;

        for(i = 0; i < ROW; i++)
        {
                for(j = 0; j < COL; j++)
                {
                        printf("%7d", graph[i][j]);
                }
                printf("\n");
        }

        printf("\n\n");
}
void freeGraph(int **graph)
{
        int i;

        for(i = 0; i < ROW; i++)
        {
                free(graph[i]);
        }

        free(graph);
}


