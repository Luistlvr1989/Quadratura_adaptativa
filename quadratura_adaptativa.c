#include "quadratura_adaptativa.h"

double fd(double x) {
	return x;
}

double f1(double x) {
	return x * x;
}

double f2(double x) {
	return sin(x);
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