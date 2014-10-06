#include <unistd.h>     /* Symbolic Constants */
#include <limits.h>     /* Limits of the Data Types */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <string.h>     /* String handling */
#include <float.h>		/* Float handling */

#include "quadratura_adaptativa.h"
#include "task_list.h"

/* START GLOBALS */
double final_range;
double result = 0.0;
double range  = 0.0;
Task_List tasks;
/* END GLOBALS */

int steps = 0;

/* Prototype of the function */
void createTask(double a, double b, double (*f)(double x));
void adaptativeQuadratureDynamic(double (*f)(double x));

int main(int argc, char *argv[]) {
	char* p;
	double a, b;
	int n_threads, function = 0;

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
	if(errno != 0 || *p != '\0' || b > DBL_MAX) {
		printf("The number b is not valid\n");
		exit(EXIT_FAILURE);
	}

	errno = 0;
	n_threads = strtol(argv[3], &p, 10);
	if(errno != 0 || *p != '\0' || n_threads > INT_MAX) {
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

	double (*f)(double) = selectFunction(function);
	tasks = createTaskList();
	createTask(a, b, f);
	final_range = b - a;

	#pragma omp parallel num_threads(n_threads)
	adaptativeQuadratureDynamic(f);

	printf("The result is: %f\n", result);

	return 0;
}

void createTask(double a, double b, double (*f)(double x)) {
	Task task;

	task.local_a  = a;
	task.local_b  = b;
	task.local_fa = f(a);
	task.local_fb = f(b);
	task.local_area = trapezoidalRule(a, b, task.local_fa, task.local_fb);
	//task.local_area = (task.local_fa + task.local_fb) * (b - a) / 2.0;

	tasks.insertTask(&tasks, task);
}

void adaptativeQuadratureDynamic(double (*f)(double x)) {
	double local_a;
	double local_b;
	double local_mid;
	double local_fa;
	double local_fb;
	double local_fmid;

	Task task;
	double local_area;
	double l_area;
	double r_area;
	double error;
	int empty;

	while(TRUE) {
		#pragma omp critical
		{
			empty = tasks.isTaskListEmpty(&tasks);
			if(!empty)
				task = tasks.removeTask(&tasks);

			steps++;
		}

		printf("STEP: %d\n", tasks.length(&tasks));

		printTask(task);

		if(steps == 100)
			break;

		if(empty || final_range <= range )
			break;

		local_a    = task.local_a;
		local_b    = task.local_b;
		local_mid  = (local_a + local_b) / 2.0;
		local_fa   = task.local_fa;
		local_fb   = task.local_fb;
		local_fmid = f(local_mid);
		local_area = task.local_area;

		l_area = trapezoidalRule(local_a,   local_mid, local_fa,   local_fmid);
		r_area = trapezoidalRule(local_mid, local_b,   local_fmid, local_fb);

		error = fabs(local_area - (l_area + r_area)); 
		printf("ERROR: %f\n", error);
		if(error < ERROR) {
			#pragma omp critical 
			{
				result += local_area;
				range  += (local_b - local_a);
			}
		} else {
			#pragma omp critical 
			{	
				task.local_a    = local_a;
				task.local_b    = local_mid;
				task.local_fa   = local_fa;
				task.local_fb   = local_fmid;
				task.local_area = l_area;
				tasks.insertTask(&tasks, task);

				task.local_a    = local_mid;
				task.local_b    = local_b;
				task.local_fa   = local_fmid;
				task.local_fb   = local_fb;
				task.local_area = r_area;
				tasks.insertTask(&tasks, task);

				//printf("TASKS: %d\n\n", tasks.length(&tasks));
			}
		}
	}
}