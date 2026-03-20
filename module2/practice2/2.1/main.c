#include <stdio.h>
#include "phonebook.h"

ContactStorage* storage;

void print_brief(Contact* contact) {
  printf("%s %s\n", contact->firstname, contact->lastname);
}

void print_details(Contact* contact) {
  printf("%s\n", contact->firstname);
  printf("%s\n", contact->lastname);

  if (contact->middlename != NULL) {
    printf("%s\n", contact->middlename);
  }

  if (contact->job != NULL) {
    printf("%s\n", contact->job);
  }

  if (contact->position != NULL) {
    printf("%s\n", contact->position);
  }
}

int main() {
  storage = create_storage(10);

  Contact* contact = create_contact("john", "doe", NULL);
  Contact* contact2 = create_contact("oleg", "olegov", NULL);

  printf("%d\n", get_size(storage));
  add_contact(storage, contact);
  add_contact(storage, contact2);
  printf("%d\n", get_size(storage));

  for (size_t i = 0; i < storage->size; i++)
  {
    print_brief(storage->array[i]);
  }
  
  delete_storage(storage);
  return 0;
}