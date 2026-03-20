#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"

typedef enum {
  READ_OK = 0,
  READ_FAIL = 1,
} ReadStatus;

ReadStatus read_operator(char* value, char max_value) {
  char scan_status = 0;
  char tmp_value = 0;
  while (scan_status != 1) {
    printf("Оператор: ");
    scan_status = scanf("%hhd", &tmp_value);
    switch (scan_status) {
      case EOF:
        exit(0);
        return READ_FAIL;
      case 1:
        if (0 < tmp_value && tmp_value < max_value + 1) {
          *value = tmp_value;
          while (getchar() != '\n');
          return READ_OK;
        }
      default:
        printf("Неверный формат. Ожидается номер оператора.\n");
        while (getchar() != '\n');
        break;
    }
  }

  return READ_FAIL;
}

ReadStatus read_number(const char* prompt, double* value) {
  char scan_status = 0;
  double tmp_value = 0;
  while (scan_status < 1) {
    printf("%s", prompt);
    scan_status = scanf("%lf", &tmp_value);
    switch (scan_status) {
      case EOF:
        exit(0);
        return READ_FAIL;
      case 1:
        *value = tmp_value;
        while (getchar() != '\n');
        return READ_OK;
      default:
        printf("Неверный формат. Ожидается число.\n");
        while (getchar() != '\n');
        break;
    }
  }

  return READ_FAIL;
}

typedef struct OperationData {
  char name[30];
  operation_ptr operation;
} OperationData;

int main() {
  printf("Калькулятор\n");
  char is_running = 1;

  OperationData operations[] = {{"Сложение", add},
                                {"Вычитание", subtract},
                                {"Умножение", multiply},
                                {"Деление", divide}};

  char op_count = sizeof(operations) / sizeof(OperationData);

  while (is_running) {
    for (size_t i = 0; i < op_count; i++) {
      printf("[%d] %s\n", i + 1, operations[i]);
    }

    char op;
    ReadStatus read_status = read_operator(&op, op_count);
    if (read_status == READ_FAIL) {
      continue;
    }

    double a = 0;
    read_status = read_number("a: ", &a);
    if (read_status == READ_FAIL) {
      continue;
    }

    double b = 0;
    read_status = read_number("b: ", &b);
    if (read_status == READ_FAIL) {
      continue;
    }

    printf("= ");
    double result;
    OperationStatus status;
    status = operations[op - 1].operation(a, b, &result);

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
