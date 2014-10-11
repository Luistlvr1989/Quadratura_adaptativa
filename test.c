#include <unistd.h>     /* Symbolic Constants */
#include <limits.h>     /* Limits of the Data Types */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <string.h>     /* String handling */
#include <float.h>		/* Float handling */
#include <time.h>		/* Time handling */
#include <math.h>       /* Math Functions */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */

/* START GLOBALS */
double result = 0.0;

// Execution time
struct timespec start, finish;
double elapsed;
/* END GLOBALS */

/* Prototype of the function */
void init(int interval);

int main(int argc, char *argv[]) {
	char* p;
	int n_threads;
	int interval;

	errno = 0;
	n_threads = strtol(argv[1], &p, 10);
	if(errno != 0 || *p != '\0' || n_threads > INT_MAX || n_threads <= 0) {
		printf("The number of threads is not valid\n");
		exit(EXIT_FAILURE);
	}

	omp_set_num_threads(n_threads);
	interval = INT_MAX / n_threads;
	printf("#Starting with %d threads\n", n_threads);

	clock_gettime(CLOCK_MONOTONIC, &start);
	//init(interval);
	#pragma omp parallel
	{
		int i = omp_get_thread_num() * interval;
		int j = i + interval;
		printf("Start in %d end in %d\n", i, j);

		for(; i < j; i++);
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("The time spent is: %f\n", elapsed);


	return 0;
}

void init(int interval) {
	int start = omp_get_thread_num() * interval;
	int end   = start + interval;
	printf("Start in %d end in %d\n", start, end);

	for(; start < end; start++);
}

