// Title :: free_list.c
// Author :: Nicholas Hemstreet


#include "free_list.h"
#include<stdlib.h>
#include <stdio.h>


void print_fl_list(FL_HEADER_TYPE* list){
  printf("-------------------\n");
  printf("***FREE LIST HEADER***\n");
  FL_HEADER_TYPE* node = list;
  while (node != NULL){
    printf("-------------------\n");
    printf("ADDR:: %lu\n",node);
    printf("\tnext:: %lu\n",node->next);
    printf("\tprev:: %lu\n",node->prev);
    printf("\tlev :: %i\n",node->level);
    printf("-------------------\n");
    node = node->next;
  }

}

FL_HEADER_TYPE* add_free_list(FL_HEADER_TYPE* alloc, FL_HEADER_TYPE* new_head ){
  if(new_head != NULL) {
    if (alloc == NULL){
      alloc = new_head;
      new_head->prev = NULL;
      new_head->next = NULL;
      return alloc;
    }
    FL_HEADER_TYPE* ol_head = alloc;
    alloc= new_head;
    alloc->prev = NULL;
    alloc->next = ol_head;
    ol_head->prev = alloc;
    return alloc;
  }else{
    return alloc;
  }
}

FL_HEADER_TYPE* remove_free_list(FL_HEADER_TYPE* alloc , FL_HEADER_TYPE* fl_head) {
  // Remove an item from the free list on _alloc
  FL_HEADER_TYPE* head = alloc;
  while (head != NULL ){
    if (head == fl_head ){
      // get the previous and the next
      if (head->prev == NULL && head->next == NULL ){
        // it's the allocator head so we just set ourselves to null
        alloc= NULL;
      }else if (head->prev != NULL && head->next == NULL){
        //  End of the list
        head->prev->next = NULL;
      }else if (head->prev == NULL &&  head->next != NULL){
        // replacing the allocator head
        alloc = head->next;
        alloc->prev = NULL;
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
      return alloc;
    }else{
      head = head->next;
    }
  } 
  return alloc;
}

