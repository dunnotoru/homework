#include <dlfcn.h>
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"

typedef enum { READ_OK = 0, READ_FAIL = 1, READ_EXIT } ReadStatus;

ReadStatus read_operator(char* value, char max_value) {
  char scan_status = 0;
  char tmp_value = 0;
  while (scan_status != 1) {
    printf("Оператор (Q - выход): ");
    scan_status = scanf("%c", &tmp_value);
    switch (scan_status) {
      case EOF:
        exit(0);
        return READ_FAIL;
      case 1:
        if (tmp_value == 'Q') {
          return READ_EXIT;
        }
        
        char n = tmp_value - 48;
        if (0 < n && n < max_value + 1) {
          *value = n;
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

int main() {
  printf("Калькулятор\n");
  char is_running = 1;

  struct LibData {
    void* handle;
    char* name;
    operation_ptr operation;
  };

  char* libs[4] = {"libsum.so", "libsubtract.so", "libmultiply.so",
                   "libdivide.so"};

  struct LibData data[4] = {0};

  int error = 0;
  for (size_t i = 0; i < 4; i++) {
    data[i].handle = dlopen(libs[i], RTLD_NOW);
    if (data[i].handle == NULL) {
      fprintf(stderr, "Ошибка при загрузке динамической библиотеки. %s\n",
              dlerror());
      error = 1;
      break;
    }

    char* (*get_name)(void) = dlsym(data[i].handle, "get_name");
    data[i].name = get_name();
    data[i].operation = dlsym(data[i].handle, "operation");
  }

  if (error) {
    for (size_t i = 0; i < 4; i++) {
      if (data[i].handle != NULL) {
        dlclose(data[i].handle);
      }
    }
    return 1;
  }

  char op_count = 4;

  while (is_running) {
    for (size_t i = 0; i < op_count; i++) {
      printf("[%d] %s\n", i + 1, data[i].name);
    }

    char op;
    ReadStatus read_status = read_operator(&op, op_count);
    if (read_status == READ_EXIT) {
      is_running = 0;
      break;
    }
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
    status = data[op - 1].operation(a, b, &result);

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

  for (size_t i = 0; i < 4; i++) {
    if (data[i].handle != NULL) {
      if (dlclose(data[i].handle) == 0) {
        printf("Библиотека успешно закрыта.\n");
      } else {
        fprintf(stderr, "Ошибка при закрытии библиотеки.\n");
      }
    }
  }

  return 0;
}
