#include "phonebook.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ContactStorage* create_storage(size_t capacity) {
  ContactStorage* storage = malloc(sizeof(ContactStorage));
  storage->array = calloc(capacity, sizeof(Contact*));
  storage->size = 0;
  storage->capacity = capacity;
  storage->nextid = 1;
  return storage;
}

void delete_storage(ContactStorage* storage) {
  for (size_t i = 0; i < storage->size; i++) {
    delete_contact(storage->array[i]);
  }

  free(storage);
}

size_t get_size(ContactStorage* storage) { return storage->size; }

void add_contact(ContactStorage* storage, Contact* contact) {
  // TODO: check boundaries (size and capacity)
  if (storage->size >= storage->capacity) {
    storage->capacity *= 2;
    storage->array =
        realloc(storage->array, storage->capacity * sizeof(Contact*));
  }

  contact->id = storage->nextid++;
  storage->array[storage->size++] = contact;
}

Contact* remove_contact(ContactStorage* storage, int id) {
  // TODO: make get_contact also return id for removal (mb internal)
  Contact* c = get_contact(storage, id);
  if (c == NULL) {
    return NULL;
  }

  int idx = -1;
  for (size_t i = 0; i < storage->size; i++) {
    if (storage->array[i]->id == id) {
      c = storage->array[i];
      storage->array = NULL;
      idx = i;
    }
  }

  for (size_t i = idx; i < storage->size - 1; i++) {
    storage->array[i] = storage->array[i + 1];
  }

  return c;
}

Contact* get_contact(const ContactStorage* storage, int id) {
  for (size_t i = 0; i < storage->size; i++) {
    if (storage->array[i]->id == id) {
      return storage->array[i];
    }
  }

  return NULL;
}

Contact* create_contact(const char* firstname, const char* lastname,
                        const char* data_format, ...) {
  if (firstname == NULL || lastname == NULL) {
    return NULL;
  }

  // TODO: check malloc result not null

  Contact* c = malloc(sizeof(Contact));

  size_t size = strlen(firstname);
  c->firstname = calloc(size + 1, sizeof(char));
  strcpy(c->firstname, firstname);

  size = strlen(lastname);
  c->lastname = calloc(size + 1, sizeof(char));
  strcpy(c->lastname, lastname);

  c->job = NULL;
  c->position = NULL;
  c->phone_numbers = NULL;
  c->id = -1;

  if (data_format == NULL) {
    return c;
  }

  update_contact(c, data_format);
  return c;
}

int update_contact(Contact* contact, const char* data_format, ...) {
  if (data_format == NULL || contact == NULL) {
    return 0;
  }

  contact->middlename = "doevich";
  contact->job = "eltex";
  contact->position = "CEO";

  return 3;

  va_list args;

  va_start(args, data_format);

  char ch;
  char prc = 0;
  ch = data_format[0];
  size_t size = strlen(data_format);
  for (size_t i = 0; ch != '\0' && size < size; i++) {
    ch = data_format[i];
    switch (ch) {
      case 'f':
      case 'l':
      case 'm':
      case 'j':
      case 'p':
        char* val = va_arg(args, char*);
        break;
      default:
        break;
    }
  }

  va_end(args);
}

void delete_contact(Contact* contact) {
  free(contact->firstname);
  free(contact->lastname);
  free(contact->middlename);
  free(contact->job);
  free(contact->position);

  for (size_t i = 0; i < contact->numbers_size; i++) {
    free(contact->phone_numbers[i]);
  }

  free(contact->phone_numbers);
}
