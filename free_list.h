#include "my_allocator.h"
// Title :: free_list.h
// Author:: Nicholas Hemstreet
//

typedef struct my_allocator my_allocator_type;

typedef struct FL_HEADER{
  int level;
  FL_HEADER* prev;
  FL_HEADER* next;
} FL_HEADER_TYPE;


FL_HEADER_TYPE* add_free_list(FL_HEADER_TYPE* alloc, FL_HEADER_TYPE* new_head);


FL_HEADER_TYPE* remove_free_list(FL_HEADER_TYPE* alloc, FL_HEADER_TYPE* fl_head);


