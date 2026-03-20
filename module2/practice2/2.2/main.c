#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

typedef enum {
  READ_OK = 0,
  READ_FAIL = 1,
} ReadStatus;

ReadStatus read_operator(char *value) {
  int scan_status = 0;
  char tmp_value = 0;
  while (scan_status < 1) {
    printf("Оператор (+, -, *, /): ");
    scan_status = scanf("%c", &tmp_value);
    switch (scan_status) {
    case EOF:
      exit(0);
      return READ_FAIL;
    case 1:
      if (tmp_value == '+' || tmp_value == '-' || tmp_value == '*' ||
          tmp_value == '/') {
        *value = tmp_value;
        while (getchar() != '\n');
        return READ_OK;
      }
    default:
      printf("Неверный формат. Ожидается оператор (+, -, *, /).\n");
      while (getchar() != '\n');
      break;
    }
  }

  return READ_FAIL;
}

ReadStatus read_number(const char *prompt, double *value) {
  int scan_status = 0;
  double tmp_value = 0;
  while (scan_status < 1) {
    printf("%s", prompt);
    scan_status = scanf("%lf", &tmp_value);
    switch (scan_status) {
    case 1:
      *value = tmp_value;
      while (getchar() != '\n');
      return READ_OK;
    case EOF:
      exit(0);
      return READ_FAIL;
    default:
      printf("Неверный формат. Ожидается число.\n");
      while (getchar() != '\n');
      break;
    }
  }

  return READ_FAIL;
}

int main() {
  printf("Калькулятор\n");
  char is_running = 1;
	
  while (is_running) {
    char op;
    ReadStatus or = read_operator(&op);
    if (or == READ_FAIL) {
      continue;
    }

    double a = 0;
    ReadStatus ar = read_number("a: ", &a);
    if (ar == READ_FAIL) {
      continue;
    }

    double b = 0;
    ReadStatus br = read_number("b: ", &b);
    if (br == READ_FAIL) {
      continue;
    }

		printf("%g %c %g = ", a, op, b);
		double result;
		OperationStatus status;
		switch (op) {
			case '+':
				status = add(a,b, &result);
				break;
			case '-':
				status = subtract(a,b, &result);
				break;
			case '*':
				status = multiply(a,b, &result);
				break;
			case '/':
				status = divide(a,b, &result);
				break;
		}
	
		switch (status) {
			case OP_ZERO_DIVISION:
				printf("...\nОшибка. Деление на ноль.\n");
				break;
			case OP_INFINITY:
				printf("...\nОшибка. Слишком большое число.\n");
				break;
			case OP_OK:
				printf("%.10g\n", result);
				break;
		}
  }

  return 0;
}
