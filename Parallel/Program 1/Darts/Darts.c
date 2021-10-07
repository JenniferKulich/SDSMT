 /*****************************************************************************
  * Darts.c
  * Author: Jennifer Kulich
  * Class: CSC 410
  * Due: Sept 18, 2020
  * 
  * This will "throw" darts at 1/4 of a dartboard which will have a dartboard
  * going through it (1 ft radius). I'm not very good at describing it, but
  * when you get a random x and y, the number of darts in the circle divided
  * by the number of darts thrown can be used to calculate pi- the more darts
  * thrown, the closer to pi you will get. The part of throwing the darts is
  * written in parallel and times.
  *
  * To compile: make clean; make all     then ->    ./darts {number} 
  * where number is how many darts you want to throw
  *
  ****************************************************************************/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>

#define SEED 35791246
int THREAD_COUNT = 8;

int main(int argc, char **argv)
{
	int i;
	long long int dartsThrown = 0;
	long long int dartsInCircle = 0;
	double x, y, z;
	double piOverFour;
	double pi;
	double t0, t1, totalTime;
	srand(SEED);

	int dartsToThrow = strtol(argv[1], NULL, 10);

	t0 = omp_get_wtime();
# pragma omp parallel for num_threads(THREAD_COUNT) default(none) shared(dartsToThrow, dartsThrown, dartsInCircle) private(i) reduction(+: dartsInCircle)
	for(i = 0; i < dartsToThrow; i++)
	{
		x = (double)rand()/ RAND_MAX;
		y = (double)rand() / RAND_MAX;
		
		z = x * x + y* y;
		dartsThrown++;

		if(z <= 1)
			dartsInCircle++;
	}

	t1 = omp_get_wtime();

	piOverFour = (double)dartsInCircle / (double)dartsThrown;
	pi = (double)piOverFour * 4;
	totalTime = t1 - t0;

	printf("Pi/4 = %f\n", piOverFour);
	printf("Pi = %f\n", pi);
	printf("Elapsed time = %f seconds.\n", totalTime);
	return 0;
}

