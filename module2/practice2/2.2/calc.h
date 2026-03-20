#ifndef CALC_H
#define CALC_H

typedef enum {
	OP_OK = 0,
	OP_ZERO_DIVISION = 1,
	OP_INFINITY = 2
} OperationStatus;

OperationStatus add(double a, double b, double* r);
OperationStatus subtract(double a, double b, double* r);
OperationStatus multiply(double a, double b, double* r);
OperationStatus divide(double a, double b, double* r);

#endif // !CALC_H
