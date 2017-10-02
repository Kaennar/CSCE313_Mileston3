// Title :: free_list.c
// Author :: Nicholas Hemstreet


#include "free_list.h"
#include<stdlib.h>
#include <stdio.h>


typedef struct my_allocator{
  unsigned int total_memory;
  void* start_address;
  FL_HEADER_TYPE* free_list;
}my_allocator_type;

void print_fl_list(FL_HEADER_TYPE* list){
  printf("-------------------\n");
  printf("***FREE LIST HEADER***\n");
  FL_HEADER_TYPE* node = list;
  while (node != NULL){
    printf("-------------------\n");
    printf("ADDR:: %lu\n",node);
    printf("\tnext:: %lu\n",node->next);
    printf("\tprev:: %lu\n",node->prev);
    printf("\tlen :: %i\n",node->length);
    printf("-------------------\n");
    node = node->next;
  }

}

int add_free_list(my_allocator_type* alloc, FL_HEADER_TYPE* new_head ){
  if(new_head != NULL) {
    if (alloc->free_list == NULL){
      alloc->free_list = new_head;
      new_head->prev = NULL;
      new_head->next = NULL;
      return 1;
    }
    FL_HEADER_TYPE* ol_head = alloc->free_list;
    alloc->free_list = new_head;
    alloc->free_list->prev = NULL;
    alloc->free_list->next = ol_head;
    ol_head->prev = alloc->free_list;
    return 1;
  }else{
 /*   printf("ADD TO FREE LIST FAILED\n");
    printf("Failed Node\n");
    printf("\tnode->next :: %u\n",new_head->next);
    printf("\tnode->prev :: %u \n",new_head->prev);
    printf("\tlength     :: %i \n",new_head->length);
*/
    return 0;
  }
}

int remove_free_list(my_allocator_type* alloc , FL_HEADER_TYPE* fl_head) {
  // Remove an item from the free list on _alloc
  FL_HEADER_TYPE* head = alloc->free_list;
  while (head != NULL ){
    if (head == fl_head ){
      // get the previous and the next
      if (head->prev == NULL && head->next == NULL ){
        // it's the allocator head so we just set ourselves to null
        alloc->free_list = NULL;
      }else if (head->prev != NULL && head->next == NULL){
        //  End of the list
        head->prev->next = NULL;
      }else if (head->prev == NULL &&  head->next != NULL){
        // replacing the allocator head
        alloc->free_list = head->next;
        alloc->free_list->prev = NULL;
      }else {
        // both are not NULL
        head->next->prev = head->prev;
        head->prev->next = head->next;
        // set both of these to NULL
      }
      head->prev    = NULL;
      head->next    = NULL;
      //printf ("Looking to remove:: %u\n",head);
      //print_fl_list(alloc->free_list); 
      return 1;
    }else{
      head = head->next;
    }
  }
    
    
  return 0;
}

