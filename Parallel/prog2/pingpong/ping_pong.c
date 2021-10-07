 /*****************************************************************************
  * ping_pong.c
  * Author: Jennifer Kulich
  * Class: CSC 410
  * Due: Nov 16, 2020
  * 
  * This will send a message from process 0 to process 1 and then back to process
  * 0. This will be done using a blocking send/receive, a non-blocking 
  * send/receive, and the MPI_Sendrecv. Multiple roundtrips of different size 
  * messages may be made and timed. Each timing will be printed out
  *
  * To compile: make clean; make all     
  *    then -> mpiexec -np 2 ./ping_pong {number of times to run} {size Message} 
  *
  ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;
#define MAX 2
#define MAX_ORDER 100

int main(int argc, char * argv[])
{
        int numberTimesToPass = atoi(argv[1]);
        int sizeMessage = atoi(argv[2]);

        char * message = malloc(sizeof(char ) * sizeMessage);

        int comm_sz;
        int my_rank;
        int nameLength = 0;
        char hostName[MPI_MAX_PROCESSOR_NAME];
        char recvHostName[2][MPI_MAX_PROCESSOR_NAME];

        MPI_Status status;
        MPI_Request sendRequest;
        MPI_Request recvRequest;

        double elapsedTime = 0;
        double start = 0;
        double finish= 0;

        /* Start up MPI */
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

        MPI_Get_processor_name(hostName, &nameLength);
        MPI_Gather(&hostName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, &recvHostName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);

	if(my_rank == 0)
        {
                //BLOCKING
                start = MPI_Wtime();
                for(int i = 0; i < numberTimesToPass; i++)
                {
                        MPI_Send(message, sizeMessage, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
                        MPI_Recv(message, sizeMessage, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
                }
                finish = MPI_Wtime();

                elapsedTime = ((finish - start) * 1000000) / (2 * numberTimesToPass);
                printf("\nBLOCKING: ");
                printf("Size of message: %d with an average elapsed time: %f microseconds\n\n", sizeMessage, elapsedTime);


                //NON BLOCKING
                start = MPI_Wtime();
                for(int n = 0; n < numberTimesToPass; n++)
                {
                        MPI_Isend(message, sizeMessage, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &sendRequest);
                        MPI_Irecv(message, sizeMessage, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &recvRequest);
                        MPI_Wait(&sendRequest, &status);
                        MPI_Wait(&recvRequest, &status);
                }
                finish = MPI_Wtime();

                elapsedTime = ((finish - start) * 1000000) / (2 * numberTimesToPass);
                printf("NON BLOCKING: ");
                printf("Size of message: %d with an average  elapsed time: %f microseconds\n\n", sizeMessage, elapsedTime);

                //MPI_SENDRECV
                start = MPI_Wtime();
                for(int m = 0; m < numberTimesToPass; m++)
                {
                        MPI_Sendrecv(message, sizeMessage, MPI_CHAR, 1, 1, message, sizeMessage, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
                }

                finish = MPI_Wtime();
                elapsedTime = ((finish - start) * 1000000) / ( 2 * numberTimesToPass);
                printf("SEND/RECEIVE: ");
                printf("Size of message: %d with an average elapsed time: %f microseconds\n\n", sizeMessage, elapsedTime);
        }
else
        {
                //BLOCKING
                for(int i = 0; i < numberTimesToPass; i++)
                {
                        MPI_Recv(message, sizeMessage, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
                        MPI_Send(message, sizeMessage, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
                }

                //NON BLOCKING
                for(int n = 0; n < numberTimesToPass; n++)
                {
                        MPI_Irecv(message, sizeMessage, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &sendRequest);
                        MPI_Isend(message, sizeMessage, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &recvRequest);
                        MPI_Wait(&sendRequest, &status);
                        MPI_Wait(&recvRequest, &status);
                }

                //MPI_SENDRECV
                for(int m = 0; m < numberTimesToPass; m++)
                {
                        MPI_Sendrecv(message, sizeMessage, MPI_CHAR, 0, 1, message, sizeMessage, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
                }
        }



        /* Shut down MPI */
        MPI_Finalize();

        return 0;
}

