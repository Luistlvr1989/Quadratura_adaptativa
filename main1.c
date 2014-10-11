#include <unistd.h> 	/* Symbolic Constants */
#include <limits.h> 	/* Limits of the Data Types */
#include <sys/types.h> 	/* Primitive System Data Types */
#include <errno.h> 		/* Errors */
#include <string.h> 	/* String handling */
#include <float.h> 		/* Float handling */
#include <time.h>		/* Time handling */

#include "quadratura_adaptativa.h"

/* START GLOBALS */
double result = 0.0;
// Execution time
struct timespec start, finish;
double elapsed;
/* END GLOBALS */

/* Prototype of the function */
void init(double a, double b, double interval, double (*f)(double x));

int main(int argc, char *argv[]) {
	char* p;
	int n_threads, function = 0, i;
	double a, b;

	if(argc <= 3) {
		printf("Specify a, b, the number of threads, and the function(optional)\n");
		exit(EXIT_FAILURE);
	}
	
	errno = 0;
	a = strtod(argv[1], &p);
	if(errno != 0 || *p != '\0' || a > DBL_MAX) {
		printf("The number a is not valid\n");
		exit(EXIT_FAILURE);
	}

	errno = 0;
	b = strtod(argv[2], &p);
	if(errno != 0 || *p != '\0' || b > DBL_MAX || b < a) {
		printf("The number b is not valid\n");
		exit(EXIT_FAILURE);
	}
	
	errno = 0;
	n_threads = strtol(argv[3], &p, 10);
	if(errno != 0 || *p != '\0' || n_threads > INT_MAX || n_threads <= 0) {
		printf("The number of threads is not valid\n");
		exit(EXIT_FAILURE);
	}
	
	if(argc > 4) {
		errno = 0;
		function = strtol(argv[4], &p, 10);
		if(errno != 0 || *p != '\0' || function > INT_MAX) {
			printf("The function number is not valid\n");
			exit(EXIT_FAILURE);
		}
	}
	
	omp_set_num_threads(n_threads);
	double (*f)(double) = selectFunction(function);
	double interval = (b - a) / n_threads;
	printf("#Starting function %d with [%.2f - %.2f] and %d threads\n", function, a, b, n_threads);
	
	for(i = 0; i < 25; i++) {
		result = 0.0;
		clock_gettime(CLOCK_MONOTONIC, &start);
		
		#pragma omp parallel
		init(a, b, interval, f);
	
		//printf("The result is: %f\n", result);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		elapsed = (finish.tv_sec - start.tv_sec);
		elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
		//printf("The time spent is: %f\n", elapsed);
		printf("%f\n", elapsed);
	}
	return 0;
}
void init(double a, double b, double interval, double (*f)(double x)) {
	double local_a = a + omp_get_thread_num() * interval;
	double local_b = local_a + interval;
	double local_fa = f(local_a);
	double local_fb = f(local_b);	
	double local_result = adaptativeQuadrature(local_a, local_b, local_fa, local_fb, f);
	
	#pragma omp atomic
	result += local_result;
}