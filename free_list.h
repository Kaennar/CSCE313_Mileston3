#include "my_allocator.h"
// Title :: free_list.h
// Author:: Nicholas Hemstreet
//

typedef struct my_allocator my_allocator_type;

typedef struct FL_HEADER{
  int length;
  FL_HEADER* prev;
  FL_HEADER* next;
} FL_HEADER_TYPE;


int add_free_list(my_allocator_type* list, FL_HEADER_TYPE* new_head);


int remove_free_list(my_allocator_type* list, FL_HEADER_TYPE* fl_head);


