#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <stdlib.h>
#include "linked_list.h"

#define FIELD_SIZE 64
#define NUMBERS_SIZE 5

//f,l,m,j,p,
typedef struct Contact {
  int id;
  char* firstname;
  char* lastname;
  char* middlename;
  char* job;
  char* position;

  char phone_numbers[NUMBERS_SIZE][FIELD_SIZE];
} Contact;

typedef struct ContactStorage {
  List* list;
  size_t size;
  size_t nextid;
} ContactStorage;

ContactStorage* create_storage();
void delete_storage(ContactStorage* storage);

void add_contact(ContactStorage* storage, Contact* contact);
Contact* get_contact(const ContactStorage* storage, int id);
void remove_contact(ContactStorage* storage, Contact* contact);

Contact* create_contact(const char* firstname, const char* lastname,
                        const char* data_format, ...);
int update_contact(Contact* contact, const char* data_format, ...);
void delete_contact(Contact* contact);

#endif
