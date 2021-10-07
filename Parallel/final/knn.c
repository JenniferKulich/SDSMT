/*****************************************************************************
  * knn.c
  * Author: Jennifer Kulich
  * Class: CSC 410
  * Due: Dec 7, 2020
  * 
  * This will implemen the knn algorithm and find the classification using 
  * MPI. A better description is in the pdf. 
  *
  * To run: mpiexec ./knn {k} {data set file} {query file}
  *
  *
  ****************************************************************************/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int K = 0;

double **ParseFile(FILE * fileName, double **data, int numberLines, int dataPerLine);
int GetNumberLines(FILE * file);
int GetNumberDataPerLine(FILE * file);

int main(int argc, char **argv)
{
        int numTasks, rank;
        char * csvFileName;
        char * queryFileName;
        int csvNumberLines, csvDataPerLine;
        double **csvData;
        double *queryData;
        double minDistanceClassification = 0;
	MPI_Status status;

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if(argc == 4)
        {
                K = atoi(argv[1]);
                csvFileName = argv[2];
                queryFileName = argv[3];
        }
        else
        {
                printf("Did not provice correct number or arguments\n");
                printf("Please refer to documentation for how to run\n");
        }

        if(rank == 0)
        {
                //open csv file and read in
                FILE * csvFile = fopen((const char *)csvFileName, "r");
                csvNumberLines = GetNumberLines(csvFile);
                csvDataPerLine = GetNumberDataPerLine(csvFile);
                csvDataPerLine += 1;
                csvData = (double**)malloc(csvNumberLines * sizeof(double));
                for(int i = 0; i < csvNumberLines; i++)
                {
                        csvData[i] = (double*)malloc(csvDataPerLine * sizeof(double));
                }
                csvData = ParseFile(csvFile, csvData, csvNumberLines, csvDataPerLine);
		//open query file and read in
                //this is repeated code, but I couldn't get it working unless I did it this way
                FILE * queryFile = fopen((const char *)queryFileName, "r");
                queryData = malloc((csvDataPerLine - 2)* sizeof(double));


                char *line;
                size_t something;
                char *item;
                getline(&line, &something, queryFile);
                item = strtok(line, ",");
                for(int m = 0; m  < csvDataPerLine - 2; m++)
                {
                        queryData[m] = atof(item);
                        item = strtok(NULL, ",");
                }
                fclose(queryFile);

                int dataPerTask = csvNumberLines / numTasks;
                int amountToSendToTask = dataPerTask;
                int remainder = csvNumberLines % numTasks;

                //send data to proper threads
                for(int n = 0; n < numTasks; n++)
                {
                        //send remaining data points to last task
                        if(n == numTasks - 1)
                        {
                                amountToSendToTask += remainder;
                        }
			//send remaining data points to last task
                        if(n == numTasks - 1)
                        {
                                amountToSendToTask += remainder;
                        }

                        //send how many tasks will be sent
                        MPI_Send((const void *)&amountToSendToTask, 1, MPI_INT, n, 0, MPI_COMM_WORLD);
                        //send how much data per line there will be
                        MPI_Send((const void *)&csvDataPerLine, 1, MPI_INT, n, 0, MPI_COMM_WORLD);
                        //send data
                        for(int i =  n * dataPerTask; i < n * dataPerTask + amountToSendToTask; i++)
                        {
                                for(int j = 0; j < csvDataPerLine; j++)
                                {
                                        MPI_Send(&csvData[i][j], 1, MPI_DOUBLE, n, 0, MPI_COMM_WORLD);
                                }
                        }

                        //also send query
                        //MPI_Send((const void *)&queryDataPerLine, 1, MPI_INT, n, 0, MPI_COMM_WORLD);
                        for(int i = 0; i < csvDataPerLine - 2; i++)
                        {
                                MPI_Send(&queryData[i], 1, MPI_DOUBLE, n, 0, MPI_COMM_WORLD);
                        }
                }
        }

        MPI_Barrier(MPI_COMM_WORLD);

	double number;
        int numberDataRows;
        int csvNumberDataPerLine;
        double **dataToCalculate;
        double *query;

        //get how many rows of data will be received
        MPI_Recv(&numberDataRows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Recv(&csvNumberDataPerLine, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Barrier(MPI_COMM_WORLD);

        //allocate data

        dataToCalculate = malloc(numberDataRows * sizeof(double));
        for(int i = 0; i < numberDataRows; i++)
        {
                dataToCalculate[i] = malloc(csvNumberDataPerLine * sizeof(double));
        }

        MPI_Barrier(MPI_COMM_WORLD);

        //get data 
        for(int i = 0; i < numberDataRows; i++)
        {
                for(int j = 0; j < csvNumberDataPerLine; j++)
                {
                        MPI_Recv(&number, 1, MPI_DOUBLE, 0,0, MPI_COMM_WORLD, &status);
                        dataToCalculate[i][j] = number;
                }
	}

        MPI_Barrier(MPI_COMM_WORLD);

        //get query
        query = malloc((csvNumberDataPerLine - 2) * sizeof(double));
        for(int i = 0; i < csvNumberDataPerLine -2; i++)
        {
                MPI_Recv(&number, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
                query[i] = number;
        }
        //MPI_Recv(query, queryNumberDataPerLine, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Barrier(MPI_COMM_WORLD);


        //now for the fun part: finding the distance
//      double ** classifications = malloc(numberDataRows * sizeof(double));
//      for(int i = 0; i < numberDataRows; i++)
//      {
        //      classifications[i] = malloc(2 * sizeof(double));
        //}

        //MPI_Barrier(MPI_COMM_WORLD);  
        //calcualte the distance from query
        for(int i = 0; i < numberDataRows; i++)
        {
                for(int j = 0; j < csvNumberDataPerLine - 2; j++)
                {
                        dataToCalculate[i][csvNumberDataPerLine] += (dataToCalculate[i][j] - query[j]) * (dataToCalculate[i][j] - query[j]);
                }
 		dataToCalculate[i][csvNumberDataPerLine] = sqrt(dataToCalculate[i][csvNumberDataPerLine]);
        }

        MPI_Barrier(MPI_COMM_WORLD);

        double classification = 100;
        double distance = 100;

        MPI_Barrier(MPI_COMM_WORLD);

        //double *classificationCount;
        //classificationCount = (double *)malloc(numberClassifications * sizeof(double));
        //for(int i = 0; i < numberClassifications; i++)
        //      classificationCount[i] = 0

        for(int i = 0; i < K; i++)
        {
                //find minimum in each rank
                for(int n = 0; n < numberDataRows; n++)
                {
                        if(n == 0)
                        {
                                classification = dataToCalculate[0][csvNumberDataPerLine - 2];
                                distance = dataToCalculate[0][csvNumberDataPerLine];
                        }
                        else
                        {
                                if(dataToCalculate[n][csvNumberDataPerLine] < distance)
                                {
                                classification = dataToCalculate[n][csvNumberDataPerLine - 2];
                                        distance = dataToCalculate[n][csvNumberDataPerLine];
                                }
                        }
		}

                MPI_Barrier(MPI_COMM_WORLD);
                MPI_Allreduce(&classification, &minDistanceClassification, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
                MPI_Barrier(MPI_COMM_WORLD);
                //classificationCount[minDistanceClassification - 1] += 1;
        }

        //find the classification with the most count and send it to rank 0

        MPI_Barrier(MPI_COMM_WORLD);

        if(rank == 0)
        {
                printf("Classification: %f\n",minDistanceClassification);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Finalize();

        return 0;
}

double **ParseFile(FILE * fileName, double **data, int numberLines, int dataPerLine)
{
        char * line;
        size_t size;
        char * item;

        //read in from file
        for(int i = 0; i < numberLines; i++)
        {
                getline(&line, &size, fileName);
                item = strtok(line, ",");
                for(int j = 0; j < dataPerLine - 1; j++)
                {
                        data[i][j] = atof(item);
                        item = strtok(NULL, ",");
                }
                data[i][dataPerLine - 1] = 0;
        }

        fclose(fileName);

        return data;
}

int GetNumberLines(FILE * file)
{
        int lineCount = 0;
        char * currentLine;
        size_t size;

        while(getline(&currentLine, &size, file) != -1)
        {
                lineCount++;
        }

        rewind(file);

        return lineCount;
}

int GetNumberDataPerLine(FILE *file)
{
        int dataPerLine = 0;
        char * currentLine;
        size_t size;

        getline(&currentLine, &size, file);

        for(int i = 0; i < (int) size; i++)
        {
                if(currentLine[i] == ',' ||  currentLine[i] == '\n')
                {
                        dataPerLine++;
                }
        }

        rewind(file);
        return dataPerLine;
}

