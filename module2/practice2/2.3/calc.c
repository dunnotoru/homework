#include "calc.h"
#include <float.h>

OperationStatus add(double a, double b, double* r) {
	*r = a + b;
	return OP_OK;
}

OperationStatus subtract(double a, double b, double* r) {
	*r = a - b;
	return OP_OK;
}

OperationStatus multiply(double a, double b, double* r) {
	*r = a * b;
	return OP_OK;
}

OperationStatus divide(double a, double b, double* r) {
	if ((b >= 0 && b < DBL_EPSILON) || (b <= 0 && b > -DBL_EPSILON)) {
		return OP_ZERO_DIVISION;
	}

	*r = a / b;
	return OP_OK;
}
