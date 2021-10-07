/*****************************************************************************
 * Prime.c
 * Author: Jennifer Kulich
 * Class: CSC 410
 * Due: Sept 18, 2020
 * 
 * This will take in a prime you want to conpute the pries to and compute the
 * primes. It does this by creating an array, and setting everything to 0. 
 * Then, in parallel, the program will use the Sieve of Eratosthenes method
 * to copute the primes. This is also times to see how long it will take. 
 *
 * To compile: make clean; make all     then ->    ./prime {number} 
 * where number is what number you want to compute your prime to
 *
 ****************************************************************************/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>

int THREAD_COUNT = 8;

void printPrimes(int *primes, int number)
{
        int counter = 0;

        for(int i = 2; i <= number; i++)
        {
                if(primes[i] != 0)
                {
                        if(counter % 10 == 0)
                                printf("\n%d: ", counter);

                        printf("%d ", primes[i]);
                        counter++;
                }
        }
        printf("\n");
}

int main(int argc, char **argv)
{
        int number, i, j;
	double t0, t1, totalTime;
	number = strtol(argv[1], NULL, 10);

        int primes[number + 1];

        //populate array
        for(int i = 2; i <= number; i++)
                primes[i] = i;

        i = 2;
	int sqrtNumber = (int)sqrt((double)number);

        t0 = omp_get_wtime();

#       pragma omp parallel for num_threads(THREAD_COUNT) default(none) shared(primes, sqrtNumber, number) private(i, j)
        for(i = 2; i  <= sqrtNumber; i++)
        {
                if(primes[i] != 0)
                {
                        for(j = i; i * j <= number; j++)
                        {
                                primes[i * j] = 0;
                        }
                }
        }
        t1 = omp_get_wtime();

        printPrimes(primes, number);
        totalTime = t1 - t0;
        printf("Elapsed time = %f seconds.\n", totalTime);

        return 0;
}

