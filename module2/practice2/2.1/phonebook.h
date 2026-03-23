#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <stdlib.h>

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

  int numbers_size;
  char * phone_numbers[NUMBERS_SIZE];
} Contact;

typedef struct ContactStorage {
  Contact** array;
  size_t size;
  size_t capacity;
  size_t nextid;
} ContactStorage;

ContactStorage* create_storage(size_t capacity);
void delete_storage(ContactStorage* storage);
size_t get_size(ContactStorage *storage);

void add_contact(ContactStorage* storage, Contact* contact);
Contact* get_contact(const ContactStorage* storage, int id);
Contact* remove_contact(ContactStorage* storage, int id);

Contact* create_contact(const char* firstname, const char* lastname,
                        const char* data_format, ...);
int update_contact(Contact* contact, const char* data_format, ...);
void delete_contact(Contact* contact);

void print_brief(Contact* contact);
void print_details(Contact* contact);

#endif
