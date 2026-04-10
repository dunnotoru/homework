#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "priority_queue.h"

#define BUFFER_SIZE 64

typedef enum MENU_STATUS {
  MENU_OK,
  MENU_ERROR,
  MENU_FATAL,
  MENU_QUIT
} MENU_STATUS;

typedef enum POP_CMP { EQ, GE } POP_CMP;

MENU_STATUS menu_add(PriorityQueue *queue);
MENU_STATUS menu_pop_cmp(PriorityQueue *queue, POP_CMP cmp);
MENU_STATUS menu_pop_head(PriorityQueue *queue);
MENU_STATUS menu_list(PriorityQueue *queue);
void cleanup(PriorityQueue *queue);

int main() {
  PriorityQueue *queue = queue_create();

  setlocale(LC_ALL, "ru_RU.UTF-8");

  while (true) {
    printf("[1] Добавить значение в очередь.\n");
    printf("[2] Получить значение с указанным приоритетом.\n");
    printf("[3] Получить значение с приоритетом не меньше указанного.\n");
    printf("[4] Получить первое значение в очереди.\n");
    printf("[5] Вывести содержимое очереди.\n");

    char buffer[BUFFER_SIZE];
    printf("> ");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      cleanup(queue);
      perror("Couldn't read input");
      exit(1);
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    char *endptr;
    int opt = strtol(buffer, &endptr, 10);
    if (*endptr != '\0' || opt < 1 || opt > 5) {
      printf("Значение должно быть числом от 1 до 4.\n");
      continue;
    }

    MENU_STATUS status = MENU_OK;
    switch (opt) {
    case 1:
      status = menu_add(queue);
      break;
    case 2:
      status = menu_pop_cmp(queue, EQ);
      break;
    case 3:
      status = menu_pop_cmp(queue, GE);
      break;
    case 4:
      status = menu_pop_head(queue);
      break;
    case 5:
      status = menu_list(queue);
      break;
    default:
      break;
    }

    if (status == MENU_QUIT) {
      break;
    }

    if (status == MENU_FATAL) {
      cleanup(queue);
      exit(1);
    }
  }

  cleanup(queue);
}

void cleanup(PriorityQueue *queue) {
  void *value = NULL;
  do {
    value = queue_pop_head(queue, NULL);
    free(value);
  } while (value != NULL);

  queue_delete(&queue);
}

MENU_STATUS menu_add(PriorityQueue *queue) {
  char value[BUFFER_SIZE];
  uint8_t priority = 0;
  printf("Значение [Q - отказаться]: ");
  char buffer[BUFFER_SIZE];
  if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
    perror("Ошибка при чтении ввода");
    return MENU_FATAL;
  }

  buffer[strcspn(buffer, "\r\n")] = '\0';

  if (strlen(buffer) == 1 && buffer[0] == 'Q') {
    return MENU_QUIT;
  }

  strncpy(value, buffer, BUFFER_SIZE);

  while (true) {
    printf("Приоритет [0 - 255], [Q - отказаться]: ");
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      perror("Ошибка при чтении ввода");
      return MENU_FATAL;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    if (strlen(buffer) == 1 && buffer[0] == 'Q') {
      return MENU_QUIT;
    }

    char *endptr;
    int tmp = strtol(buffer, &endptr, 10);
    if (*endptr != '\0' || tmp < 0 || tmp > 255) {
      printf("Ожидается число в диапазоне [0 - 255]");
      continue;
    }

    priority = (uint8_t)tmp;
    break;
  }

  size_t size = strlen(value);
  char *ptr = calloc(size, sizeof(char));
  strncpy(ptr, value, size);
  queue_push(queue, ptr, priority);

  return MENU_OK;
}

MENU_STATUS menu_pop_cmp(PriorityQueue *queue, POP_CMP cmp) {
  uint8_t priority = 0;
  while (true) {
    printf("Приоритет [0 - 255], [Q - отказаться]: ");
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      perror("Ошибка при чтении ввода");
      return MENU_FATAL;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    if (strlen(buffer) == 1 && buffer[0] == 'Q') {
      return MENU_QUIT;
    }

    char *endptr;
    int tmp = strtol(buffer, &endptr, 10);
    if (*endptr != '\0' || tmp < 0 || tmp > 255) {
      printf("Ожидается число в диапазоне [0 - 255]");
      continue;
    }

    priority = (uint8_t)tmp;
    break;
  }

  if (cmp == EQ) {
    void *value = queue_pop_eq(queue, priority);
    if (value == NULL) {
      printf("Значение с приоритетом %d не найдено.\n", priority);
    } else {
      printf("Приоритет: %d; Значение: %s\n", priority, (char *)value);
    }

    return MENU_OK;
  }

  uint8_t actual_priority = 0;
  void *value = queue_pop_ge(queue, priority, &actual_priority);
  if (value == NULL) {
    printf("Значение с приоритетом выше или равно %d не найдено.\n", priority);
  } else {
    printf("Найден приоритет: %d >= %d; Значение: %s\n", actual_priority,
           priority, (char *)value);
  }

  return MENU_OK;
}

MENU_STATUS menu_pop_head(PriorityQueue *queue) {
  uint8_t actual_priority = 0;
  void *value = queue_pop_head(queue, &actual_priority);
  if (value != NULL) {
    printf("Первый в очереди элемент: %s; Приоритет: %d.\n", (char *)value,
           actual_priority);
  } else {
    printf("Очередь пуста.\n");
  }

  return MENU_OK;
}

MENU_STATUS menu_list(PriorityQueue *queue) {
  printf("   Содержимое очереди\n");
  printf("=========================\n");
  printf("Значение\tПриоритет\n");
  QueueIterator *it = it_begin(queue);
  while (it_has_next(it)) {
    uint8_t priority = 0;
    void *value = it_next(it, &priority);
    printf("%s\t\t %d\n", (char *)value, priority);
  }

  return MENU_OK;
}
