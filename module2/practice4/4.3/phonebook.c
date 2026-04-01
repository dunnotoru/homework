#include "phonebook.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_value(Contact* c, char key, const char* value);

ContactStorage* create_storage() {
  ContactStorage* storage = malloc(sizeof(ContactStorage));
  storage->list = NULL;
  storage->size = 0;
  storage->nextid = 1;
  return storage;
}

void delete_storage(ContactStorage* storage) {
  if (storage->list == NULL) {
    free(storage);
    return;
  }

  ListIterator* it = it_begin(storage->list);

  while (it_has_next(it)) {
    Contact* c = it_remove_current(it);
    delete_contact(c);
  }

  it_delete(it);
  free(storage->list);
  free(storage);
}

int lastname_cmp(void* left, void* right) {
  return strcmp(((Contact*)left)->lastname, ((Contact*)right)->lastname);
}

void add_contact(ContactStorage* storage, Contact* contact) {
  if (storage == NULL) {
    return;
  }

  if (storage->list == NULL) {
    storage->list = list_create(lastname_cmp);
  }

  list_insert(storage->list, contact);

  contact->id = storage->nextid++;
  storage->size++;
}

void remove_contact(ContactStorage* storage, Contact* contact) {
  int retval = list_remove_value(storage->list, contact);
  if (retval) {
    storage->size--;
  }
}

Contact* get_contact(const ContactStorage* storage, int id) {
  ListIterator* it = it_begin(storage->list);
  Contact* result = NULL;
  while (it_has_next(it)) {
    Contact* c = it_next(it);
    if (id == c->id) {
      result = c;
      break;
    }
  }

  it_delete(it);

  return result;
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

  for (size_t i = 0; i < format_size; i++) {
    char* value = va_arg(args, char*);
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

  return 0;
}

void delete_contact(Contact* contact) {
  free(contact->firstname);
  free(contact->lastname);
  free(contact->middlename);
  free(contact->job);
  free(contact->position);
  free(contact);
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
    case 'n':
      strcpy(c->phone_numbers[0], value);
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
