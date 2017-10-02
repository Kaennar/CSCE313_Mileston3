/* 
    Title :: my_allocator.c
    Author:: Nicholas Hemstreet
    Brief :: Implementation of my_alloc.h
*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include "my_allocator.h"
#include "free_list.h"
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

FL_HEADER_TYPE* alloc = NULL;

// LOCAL VARIABLES
bool initialized = false;



/**
 * length = total size of the memory allocation
 *
 * basic_block_size = the minimum size of each block
 *
 *
 */
unsigned int init_allocator(unsigned int length){
  // Only initialize once
  if (!initialized){
    // set up the allocator object 
    // As always this is the length + FL_HEADER size.
    FL_HEADER_TYPE* fl_header = (FL_HEADER_TYPE*)malloc(sizeof(char)*length);
    fl_header->length =(length - sizeof(FL_HEADER_TYPE));
    fl_header->next = NULL;
    fl_header->prev = NULL;
    printf("Total Memory:: %u Free Memory:: %u",length,fl_header->length);
    printf("Size of FL_HEADER_TYPE:: %i",sizeof(FL_HEADER_TYPE));
    alloc = fl_header;
    initialized = true;
    // Nothing else.... 
    return 1;
  }else{
    return 0;
  }
}

int release_allocator(){
  if (initialized){
    // Need to recursively delete down the tree
    FL_HEADER_TYPE* head = alloc;
    while (head != NULL){
      FL_HEADER_TYPE* next = head->next;
      free(head);
      head = next;
    }
    initialized = false;
  }
}
/**
 *
 *
 *
 *
 */
void print_list(FL_HEADER_TYPE* list){
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
Addr my_malloc(int length) {
  if (initialized){
    if (alloc != NULL){
      //print_list(alloc.free_list); 
      FL_HEADER_TYPE* node = alloc;
      // traverse the list
      while (node != NULL){
        printf("Length Requested:: %i , Node length:: %i\n",length,node->length);
        // check if the length is long enough
        if (node->length >= (length)){
          void* data_ptr = (void*)((char*)node + sizeof(FL_HEADER_TYPE));
          // Remove the now assigned node
          alloc = remove_free_list(alloc, node); 
          // if there is space remaining after removing the length
          if ((int)(node->length - length -  sizeof(FL_HEADER_TYPE)) > 0){
            // Crate a new node
            int newNodeLength = node->length - length - sizeof(FL_HEADER_TYPE);  
            printf("Remaining length:: %i\n",newNodeLength);
            if (newNodeLength < 24){
              printf("Tiny Length");
            }
            FL_HEADER_TYPE* newNode = (FL_HEADER_TYPE*)((char*)node + sizeof(char)*length + sizeof(FL_HEADER_TYPE));
            // Set the variables for the newNode
            newNode->length = newNodeLength;
            // Need to set the old node to it's new length
            node->length = length;
            alloc = add_free_list(alloc,newNode);
          }
          // Get the new ptr 
          return (data_ptr);
        }else {
          // Move on
          node = node->next;
        } 
      }
    } 

  }
  return NULL;
}

int my_free(Addr a) {
  if (initialized){
    //printf("Addr to be free:: %u\n",a);
    // figure out the FL_HEADER and add it back to the free list
    FL_HEADER_TYPE*  head = (FL_HEADER_TYPE*)((char*)a - sizeof(FL_HEADER_TYPE));
    // add it to free list with the right size
    alloc = add_free_list(alloc,head);
    printf ("Adding back to free_list \n");
    print_list(alloc);
    return 1;
  }else{
    return 0;
  }
}

