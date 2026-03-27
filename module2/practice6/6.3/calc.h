#ifndef CALC_H
#define CALC_H

typedef enum {
	OP_OK = 0,
	OP_ZERO_DIVISION = 1,
	OP_INFINITY = 2
} OperationStatus;

typedef OperationStatus (*operation_ptr)(double,double, double *r);

#endif
