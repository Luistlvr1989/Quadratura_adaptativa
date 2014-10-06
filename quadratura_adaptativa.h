#ifndef QUADRATURA_ADAPTATIVA_H
#define QUADRATURA_ADAPTATIVA_H

#include <math.h>       /* Math Functions */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */

// Error threshold
#define ERROR 1e-14

// Functions
double fd(double x);
double f1(double x);
double f2(double x);

void* selectFunction(int function);

double trapezoidalRule(double a, double b, double fa, double fb);

double adaptativeQuadrature(double a, double b, double fa, double fb, double (*f)(double x));

#endif // QUADRATURA_ADAPTATIVA_H