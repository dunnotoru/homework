#include "calc.h"

char* get_name() {
  return "Умножение";
}

OperationStatus operation(double a, double b, double* r) {
  *r = a * b;
  return OP_OK;
}