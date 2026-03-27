#include "calc.h"
#include <float.h>

char* get_name() {
  return "Деление";
}

OperationStatus operation(double a, double b, double* r) {
  if ((b >= 0 && b < DBL_EPSILON) || (b <= 0 && b > -DBL_EPSILON)) {
		return OP_ZERO_DIVISION;
	}

	*r = a / b;
	return OP_OK;
}