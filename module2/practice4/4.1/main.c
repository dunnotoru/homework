#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "phonebook.h"

#define A_FIELD_COUNT 5
#define U_FIELD_COUNT 7

ContactStorage* storage;

typedef enum ReadStatus {
  READ_OK = 0,
  READ_CANCEL,
  READ_INVALID,
  READ_FAIL,
} ReadStatus;

typedef enum MenuStatus { MENU_OK = 0, MENU_FAIL } MenuStatus;

void print_brief(Contact* contact);
void print_details(Contact* contact);

ReadStatus readline(char* prompt, char* value);
ReadStatus readline_required(char* prompt, char* value);
ReadStatus read_number(char* prompt, int* value);
ReadStatus read_character(char* ch);

MenuStatus add_menu();
MenuStatus select_menu();
MenuStatus search_menu();
MenuStatus details_menu(Contact* c);
MenuStatus edit_menu(Contact* c);
MenuStatus delete_menu(Contact* c);
MenuStatus edit_numbers(Contact* c);

int main() {
  storage = create_storage();
  printf("Книга контактов!\n");
  Contact* contact = create_contact("john", "doe", NULL);
  Contact* contact2 = create_contact("oleg", "olegov", NULL);
  add_contact(storage, contact);
  add_contact(storage, contact2);

  ListIterator* it;
  int running = 1;
  while (running) {
    size_t i = 0;

    it = it_begin(storage->list);
    while (it_has_next(it)) {
      Contact* contact = it_next(it);
      printf("[%zu] ", i + 1);
      print_brief(contact);
      i++;
    }

    printf("[A] Добавить, [M] Подробнее, [S] Поиск, [Q] Завершить\n> ");

    char opt;
    ReadStatus read_status = read_character(&opt);
    switch (read_status) {
      case READ_FAIL:
        return 1;
      default:
        break;
    }

    MenuStatus menu_status = MENU_OK;
    switch (opt) {
      case 'A':
        menu_status = add_menu();
        break;
      case 'M':
        menu_status = select_menu();
        break;
      case 'S':
        menu_status = search_menu();
        break;
      case 'Q':
        running = 0;
        break;
      default:
        printf("Неизвестная опция - %c.\n", opt);
        break;
    }

    if (menu_status == MENU_FAIL) {
      running = 0;
    }
  }

  delete_storage(storage);
  return 0;
}

void print_brief(Contact* contact) {
  if (contact == NULL) {
    printf("Контакт не найден.\n");
    return;
  }

  printf("%s %s\n", contact->firstname, contact->lastname);
}

void print_details(Contact* contact) {
  if (contact == NULL) {
    printf("Контакт не найден.\n");
    return;
  }

  printf("Имя:\t\t%s\n", contact->firstname);
  printf("Фамилия:\t%s\n", contact->lastname);

  printf("Отчество:\t%s\n",
         contact->middlename != NULL ? contact->middlename : "...");
  printf("Место работы:\t%s\n", contact->job != NULL ? contact->job : "...");
  printf("Должность:\t%s\n",
         contact->position != NULL ? contact->position : "...");

  printf("Номера: \n");
  for (size_t i = 0; i < NUMBERS_SIZE; i++) {
    printf("[%ld] %s\n", i + 1,
           contact->phone_numbers[i][0] != '\0' ? contact->phone_numbers[i]
                                                : "...");
  }
}

ReadStatus readline(char* prompt, char* value) {
  printf(prompt);
  char buffer[FIELD_SIZE];
  if (fgets(buffer, FIELD_SIZE, stdin) == NULL) {
    return READ_FAIL;
  }

  if (buffer[0] == '\n') {
    return READ_CANCEL;
  }

  sscanf(buffer, "%s", value);

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

ReadStatus read_number(char* prompt, int* value) {
  printf(prompt);
  char buffer[FIELD_SIZE];
  if (fgets(buffer, FIELD_SIZE, stdin) == NULL) {
    return READ_FAIL;
  }

  if (buffer[0] == '\n') {
    return READ_CANCEL;
  }

  if (buffer[0] == 'Q') {
    return READ_CANCEL;
  }

  int number = -1;
  sscanf(buffer, "%d", &number);

  *value = number;
  return READ_OK;
}

ReadStatus read_character(char* ch) {
  char input = getchar();
  *ch = input;
  if (input == EOF) {
    return READ_FAIL;
  }

  while ((input = getchar()) != '\n' && input != EOF);

  if (input == EOF) {
    return READ_FAIL;
  }

  return READ_OK;
}

MenuStatus edit_numbers(Contact* c) {
  ReadStatus status = READ_INVALID;
  int num = -1;
  while (status != READ_OK) {
    status = read_number("Введите номер слота [1-5]: ", &num);
    switch (status) {
      case READ_FAIL:
        return MENU_FAIL;
      default:
        break;
    }

    if (0 > num || num <= 5) {
      break;
    }
  }

  char buffer[FIELD_SIZE];
  status = readline("Введите номер: ", buffer);
  if (status == READ_FAIL) {
    return MENU_FAIL;
  }

  strncpy(c->phone_numbers[num - 1], buffer, FIELD_SIZE);
  return MENU_OK;
}

MenuStatus add_menu() {
  char name[FIELD_SIZE];
  char lastname[FIELD_SIZE];

  ReadStatus status = READ_OK;
  status = readline_required("Имя*: ", name);
  if (status == READ_FAIL) {
    return MENU_FAIL;
  }

  status = readline_required("Фамилия*: ", lastname);
  if (status == READ_FAIL) {
    return MENU_FAIL;
  }

  char keys[A_FIELD_COUNT] = "nmjp";
  char format[A_FIELD_COUNT + 1] = {0, 0, 0, 0};
  char args[A_FIELD_COUNT][FIELD_SIZE] = {0};
  char prompts[A_FIELD_COUNT][FIELD_SIZE] = {
      "Телефон: ", "Отчество: ", "Место работы: ", "Должность: "};

  size_t args_size = 0;
  for (size_t i = 0; i < A_FIELD_COUNT - 1; i++) {
    char buffer[FIELD_SIZE];
    status = readline(prompts[i], buffer);
    switch (status) {
      case READ_FAIL:
        return MENU_FAIL;
      case READ_OK:
        format[args_size] = keys[i];
        strcpy(args[args_size++], buffer);
      default:
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
  return MENU_OK;
}

MenuStatus details_menu(Contact* c) {
  while (1) {
    print_details(c);
    printf(
        "[E] Изменить, "
        "[N] Изменить номер, "
        "[D] Удалить контакт, "
        "[Q] Назад\n> ");

    char opt = 0;
    if (read_character(&opt) == READ_FAIL) {
      return MENU_FAIL;
    }

    MenuStatus status = MENU_OK;

    switch (opt) {
      case 'E':
        status = edit_menu(c);
        break;
      case 'D':
        status = delete_menu(c);
        return status;
      case 'N':
        status = edit_numbers(c);
        break;
      case 'Q':
        return MENU_OK;
      default:
        printf("Неизвестная опция - %u.\n", opt);
        break;
    }

    if (status == MENU_FAIL) {
      return MENU_FAIL;
    }
  }
}

MenuStatus select_menu() {
  int id = -1;
  while (1) {
    ReadStatus status = READ_INVALID;
    while (status != READ_OK) {
      status =
          read_number("Введите номер контакта (Q чтобы отказаться): ", &id);
      switch (status) {
        case READ_FAIL:
          return MENU_FAIL;
        case READ_INVALID:
          printf("Неправильный ввод.\n");
          break;
        case READ_CANCEL:
          return MENU_OK;
        default:
          break;
      }
    }

    if (status == READ_OK && id > 0 && (size_t)id < storage->size + 1) {
      break;
    } else {
      printf("Неправильный ввод.\n");
    }
  }

  ListIterator* it = it_begin(storage->list);
  int i = 0;
  Contact* c = NULL;
  while (it_has_next(it) && i < id) {
    c = it_next(it);
    i++;
  }

  it_delete(it);

  if (i != id || c == NULL) {
    printf("Контакт не найден.\n");
    return MENU_OK;
  }

  return details_menu(c);
}

MenuStatus search_menu() {
  printf("Поиск по фамилии.\n");
  char query[FIELD_SIZE];
  ReadStatus status = readline("> ", query);
  switch (status) {
    case READ_FAIL:
      return MENU_FAIL;
    case READ_CANCEL:
      return MENU_OK;
    default:
      break;
  }

  ListIterator* it = it_begin(storage->list);
  int i = 0;
  while (it_has_next(it)) {
    Contact* c = it_next(it);
    if (strstr(c->lastname, query)) {
      i++;
      printf("[%d] ", i);
      print_brief(c);
    }
  }

  it_delete(it);

  if (i == 0) {
    printf("Контактов по такому запросу не найдено.\n");
    return MENU_OK;
  }

  int id = -1;
  while (1) {
    ReadStatus status = READ_INVALID;
    while (status != READ_OK) {
      status =
          read_number("Введите номер контакта (Q чтобы отказаться): ", &id);
      switch (status) {
        case READ_FAIL:
          return MENU_FAIL;
        case READ_INVALID:
          printf("Неправильный ввод.\n");
          break;
        case READ_CANCEL:
          return MENU_OK;
        default:
          break;
      }
    }

    if (status == READ_OK && id > 0 && id < i + 1) {
      break;
    } else {
      printf("Неправильный ввод.\n");
    }
  }

  it = it_begin(storage->list);
  i = 0;
  Contact* c = NULL;
  while (it_has_next(it) && i < id) {
    c = it_next(it);
    if (strstr(c->lastname, query)) {
      i++;
    }
  }

  printf("%d ?= %d\n", i, id);

  it_delete(it);

  if (i != id || c == NULL) {
    printf("Контакт не найден.\n");
    return MENU_OK;
  }

  return details_menu(c);
}

MenuStatus edit_menu(Contact* c) {
  ReadStatus status = READ_OK;

  char keys[U_FIELD_COUNT] = "flnmjp";
  char format[U_FIELD_COUNT] = {0, 0, 0, 0, 0, 0};
  char args[U_FIELD_COUNT][FIELD_SIZE] = {0};
  char prompts[U_FIELD_COUNT][FIELD_SIZE] = {
      "Имя: ",      "Фамилия: ",      "Телефон: ",
      "Отчество: ", "Место работы: ", "Должность: "};

  size_t args_size = 0;

  for (size_t i = 0; i < U_FIELD_COUNT - 1; i++) {
    char buffer[FIELD_SIZE];
    status = readline(prompts[i], buffer);
    switch (status) {
      case READ_FAIL:
        return MENU_FAIL;
      case READ_OK:
        format[args_size] = keys[i];
        strncpy(args[args_size++], buffer, FIELD_SIZE);
      default:
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
  return MENU_OK;
}

MenuStatus delete_menu(Contact* c) {
  while (1) {
    printf("Вы уверены? [y/n]: ");
    char opt;
    ReadStatus status = read_character(&opt);

    switch (status) {
      case READ_CANCEL:
        return MENU_OK;
      case READ_FAIL:
        return MENU_FAIL;
      case READ_INVALID:
        continue;
      default:
        break;
    }

    switch (opt) {
      case 'y':
        remove_contact(storage, c);
        return MENU_OK;
      case 'n':
        return MENU_OK;
    }
  }
}
