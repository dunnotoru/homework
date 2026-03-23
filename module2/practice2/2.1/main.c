#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "phonebook.h"

#define A_FIELD_COUNT 4
#define U_FIELD_COUNT 6

ContactStorage* storage;

void print_brief(Contact* contact) {
  printf("%s %s", contact->firstname, contact->lastname);
}

void print_details(Contact* contact) {
  printf("%s\n", contact->firstname);
  printf("%s\n", contact->lastname);

  if (contact->middlename != NULL) {
    printf("%s\n", contact->middlename);
  }

  for (size_t i = 0; i < contact->numbers_size; i++) {
    printf("[%d] %s\n", i + 1, contact->phone_numbers[i]);
  }

  if (contact->job != NULL) {
    printf("%s\n", contact->job);
  }

  if (contact->position != NULL) {
    printf("%s\n", contact->position);
  }
}

typedef enum ReadStatus {
  READ_OK = 0,
  READ_CANCEL,
  READ_FAIL,
} ReadStatus;

ReadStatus readline(char* prompt, char* value) {
  printf(prompt);
  if (fgets(value, FIELD_SIZE, stdin) == NULL) {
    return READ_FAIL;
  }

  sscanf(value, "%s", value);

  if (value[0] == '\n') {
    return READ_CANCEL;
  }

  return READ_OK;
}

ReadStatus readline_required(char* prompt, char* value) {
  ReadStatus status = READ_CANCEL;
  while (status != READ_OK) {
    status = readline(prompt, value);
    if (status == READ_FAIL) {
      return status;
    }
  }

  return READ_OK;
}

typedef enum MenuStatus { MENU_OK = 0, MENU_FAIL } MenuStatus;

int add_menu();
int edit_menu(Contact* c);
int delete_menu(Contact* c);

int add_menu() {
  char name[FIELD_SIZE];
  char lastname[FIELD_SIZE];

  ReadStatus status = READ_OK;
  status = readline_required("Имя*: ", name);
  if (status == READ_FAIL) {
    return EOF;
  }

  status = readline_required("Фамилия*: ", lastname);
  if (status == READ_FAIL) {
    return EOF;
  }

  char keys[A_FIELD_COUNT] = "nmjp";
  char format[A_FIELD_COUNT + 1] = {0, 0, 0, 0};
  char args[A_FIELD_COUNT][FIELD_SIZE] = {0, 0, 0, 0};
  char prompts[A_FIELD_COUNT][FIELD_SIZE] = {
      "Телефон: ", "Отчество: ", "Место работы: ", "Должность: "};

  size_t args_size = 0;

  for (size_t i = 0; i < A_FIELD_COUNT; i++) {
    char buffer[FIELD_SIZE];
    status = readline(prompts[i], buffer);
    switch (status) {
      case READ_FAIL:
        return EOF;
        break;
      case READ_OK:
        format[args_size] = keys[i];
        strcpy(args[args_size++], buffer);
        break;
    }
  }

  Contact* c;
  switch (args_size) {
    case 0:
      c = create_contact(name, lastname, NULL);
      break;
    case 1:
      c = create_contact(name, lastname, format, args[0]);
      break;
    case 2:
      c = create_contact(name, lastname, format, args[0], args[1]);
      break;
    case 3:
      c = create_contact(name, lastname, format, args[0], args[1], args[2]);
      break;
    case 4:
      c = create_contact(name, lastname, format, args[0], args[1], args[2],
                         args[3]);
      break;
  }

  add_contact(storage, c);

  printf("\n");
}

ReadStatus read_character(char* ch) {
  char input = getchar();
  *ch = input;
  if (input == EOF) {
    return READ_FAIL;
  }
  while ((input = getchar()) != '\n' && input != EOF) {
    printf("clearing input yoo\n");
  }
  if (input == EOF) {
    return READ_FAIL;
  }

  return READ_OK;
}

int details_menu() {
  int idx;
  while (1) {
    printf("Введите номер контакта (Q чтобы отказаться): ");
    int status = scanf("%d", &idx);
    if (status == EOF) {
      return MENU_FAIL;
    }

    if (status == 0) {
      char opt = 0;
      if (read_character(&opt) == READ_FAIL) {
        return MENU_FAIL;
      }

      if (opt == 'Q') {
        return MENU_OK;
      }
    }

    if (status == 1 && idx > 0 && idx < storage->size + 1) {
      break;
    } else {
      printf("Неверный ввод.\n");
    }
  }

  idx -= 1;

  while (1) {
    print_details(storage->array[idx]);
    printf("[E] Изменить, [D] Удалить, [Q] Назад\n> ");

    char opt = 0;
    if (read_character(&opt) == READ_FAIL) {
      return MENU_FAIL;
    }

    MenuStatus status = MENU_OK;

    switch (opt) {
      case 'E':
        status = edit_menu(storage->array[idx]);
        break;
      case 'D':
        status = delete_menu(storage->array[idx]);
        break;
      case 'Q':
        return MENU_OK;
      default:
        printf("Неизвестная опция.\n");
        break;
    }

    if (status == MENU_FAIL) {
      return MENU_FAIL;
    }
  }
}

int edit_menu(Contact* c) {
  ReadStatus status = READ_OK;

  char keys[U_FIELD_COUNT] = "flnmjp";
  char format[U_FIELD_COUNT] = {0, 0, 0, 0, 0, 0};
  char args[U_FIELD_COUNT][FIELD_SIZE] = {0, 0, 0, 0, 0, 0};
  char prompts[U_FIELD_COUNT][FIELD_SIZE] = {
      "Имя: ",      "Фамилия: ",      "Телефон: ",
      "Отчество: ", "Место работы: ", "Должность: "};

  size_t args_size = 0;

  for (size_t i = 0; i < U_FIELD_COUNT; i++) {
    char buffer[FIELD_SIZE];
    status = readline(prompts[i], buffer);
    switch (status) {
      case READ_FAIL:
        return MENU_FAIL;
      case READ_OK:
        format[args_size] = keys[i];
        strcpy(args[args_size++], buffer);
        break;
    }
  }

  switch (args_size) {
    case 1:
      update_contact(c, format, args[0]);
      break;
    case 2:
      update_contact(c, format, args[0], args[1]);
      break;
    case 3:
      update_contact(c, format, args[0], args[1], args[2]);
      break;
    case 4:
      update_contact(c, format, args[0], args[1], args[2], args[3]);
      break;
    case 5:
      update_contact(c, format, args[0], args[1], args[2], args[3], args[4]);
      break;
    case 6:
      update_contact(c, format, args[0], args[1], args[2], args[3], args[4],
                     args[5]);
      break;
  }

  printf("\n");
}

int delete_menu(Contact* c) {
  while (1) {
    printf("Вы уверены? [y/n]: ");
    char opt = getchar();
    switch (opt) {
      case 'y':
        remove_contact(storage, c->id);
        return MENU_OK;
      case 'n':
        return MENU_OK;
      case EOF:
        return MENU_FAIL;
    }
  }
}

int main() {
  storage = create_storage(10);
  printf("Книга контактов!\n");
  Contact* contact = create_contact("john", "doe", NULL);
  Contact* contact2 = create_contact("oleg", "olegov", NULL);
  add_contact(storage, contact);
  add_contact(storage, contact2);

  int running = 1;
  while (running) {
    for (size_t i = 0; i < storage->size; i++) {
      printf("[%d] ", i + 1);
      print_brief(storage->array[i]);
      printf("\n");
    }

    printf("[A] Добавить, [M] Подробнее, [Q] Завершить\n> ");
    char opt = getchar();
    if (opt == EOF) {
      running = 0;
      break;
    }

    while (getchar() != '\n');

    int status = 0;
    switch (opt) {
      case 'A':
        status = add_menu();
        break;
      case 'M':
        status = details_menu();
        break;
      case 'Q':
        running = 0;
        break;
      default:
        +printf("Неизвестная опция.\n");
        break;
    }

    if (status == EOF) {
      running = 0;
    }
  }

  delete_storage(storage);
  return 0;
}