#include "phonebook.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_value(Contact* c, char key, const char* value);

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

  Contact* c = calloc(1, sizeof(Contact));
  if (c == NULL) {
    return NULL;
  }

  size_t size = strlen(firstname);
  c->firstname = calloc(size + 1, sizeof(char));
  strcpy(c->firstname, firstname);

  size = strlen(lastname);
  c->lastname = calloc(size + 1, sizeof(char));
  strcpy(c->lastname, lastname);

  c->middlename = NULL;
  c->job = NULL;
  c->position = NULL;
  c->id = -1;

  if (data_format == NULL) {
    return c;
  }

  va_list args;
  va_start(args, data_format);
  size_t format_size = strlen(data_format);
  printf("format size: %d \n", format_size);

  for (size_t i = 0; i < format_size; i++) {
    char* value = va_arg(args, char*);
    printf("%d %s\n", i, value);
    update_value(c, data_format[i], value);
  }

  va_end(args);
  return c;
}

int update_contact(Contact* c, const char* data_format, ...) {
  if (data_format == NULL || c == NULL) {
    return 0;
  }

  va_list args;
  va_start(args, data_format);
  size_t format_size = strlen(data_format);

  for (size_t i = 0; i < format_size; i++) {
    char* value = va_arg(args, char*);
    update_value(c, data_format[i], value);
  }

  va_end(args);
}

void delete_contact(Contact* contact) {
  free(contact->firstname);
  free(contact->lastname);
  free(contact->middlename);
  free(contact->job);
  free(contact->position);
}

void update_value(Contact* c, char key, const char* value) {
  size_t size = (strlen(value) + 1) * sizeof(char);
  switch (key) {
    case 'f':
      c->firstname = realloc(c->firstname, size);
      strcpy(c->firstname, value);
      break;
    case 'l':
      c->lastname = realloc(c->lastname, size);
      strcpy(c->lastname, value);
      break;
    case 'm':
      c->middlename = realloc(c->middlename, size);
      strcpy(c->middlename, value);
      break;
    case 'j':
      c->job = realloc(c->job, size);
      strcpy(c->job, value);
      break;
    case 'p':
      c->position = realloc(c->position, size);
      strcpy(c->position, value);
      break;
  }
}
