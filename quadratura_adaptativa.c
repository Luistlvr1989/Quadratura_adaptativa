#include "quadratura_adaptativa.h"

// [1-12]
double fd(double x) {
	int i, j;
	double value = 0.0;

	for(i = 0; i < 10; i++) {
		value += pow(x, 2.0) + sin(x) * i;
	}

	return value;
}

// [5-10]
double f1(double x) {
	int i;
	double value = 0.0, value1, value2;

	for(i = 0; i < 3; i++) {
		value1 = (log(x) + pow(x, 2.0)) * cos(x);
		value2 = x * sin(x);

		value += value1 * value2;
	}
	
	return value;
}

// [2-34]
double f2(double x) {
	int i, j;
	double value = 0.0;
	
	for(i = 0; i < 5; i++) {
		value += pow(x, 2.0) * sqrt(x);
		
		for(j = 0; j < 3; j++);
			value += cos((double)j) * sin((double)j);
	}

	return value;
}

void* selectFunction(int function) {
	switch(function) {
		case 1:
			return f1;

		case 2:
			return f2;

		default:
			return fd;
	}
}

double trapezoidalRule(double a, double b, double fa, double fb) {
	return (b - a) * (fa + fb) / 2.0;
}

double adaptativeQuadrature(double a, double b, double fa, double fb, double (*f)(double x)) {
	double mid  = (a + b) / 2.0;
	double fmid = f(mid); 

	double general_area = trapezoidalRule(a, b, fa, fb);
	double sum_sub_area = trapezoidalRule(a, mid, fa, fmid) + trapezoidalRule(mid, b, fmid, fb);

	double error = fabs(general_area - sum_sub_area);
	if(error < ERROR)
		return general_area;
	return adaptativeQuadrature(a, mid, fa, fmid, f) + adaptativeQuadrature(mid, b, fmid, fb, f);
}