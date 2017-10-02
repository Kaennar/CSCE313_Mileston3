/* 
    File: memtest.c

    Author: R. Bettati
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : 16/06/27

    Test program for the memory allocator in MP1 of CSCE 313.
    This program reads and processes the command-line arguments and then
    exercises the memory allocator by allocating and de-allocating memory
    as part of the recursive invocations in the function 'ackerman', which 
    is famous for being very highly recursive.
*/
/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */
    
/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/time.h>
#include<assert.h>

#include "my_allocator.h"



void print_time_diff(struct timeval * tp1, struct timeval * tp2);
/* Prints to stdout the difference, in seconds and museconds, between
   two timevals. 
*/

int main(int argc, char ** argv) {
  //  
  //  initialize the allocator
  printf("Allocating 1000 bytes of memory to the allocator\n");
  if (init_allocator(500)){
    void* arr[100];
    int index = 0;
    int total_memory_allocated = 0;
    void* ptr = my_malloc(sizeof(int));
    while(ptr != NULL){
      *(int*)ptr = index;
      total_memory_allocated += sizeof(int);
      arr[index] = ptr;
      index += 1;
      ptr = my_malloc(sizeof(int));
    }
    printf("Total Memory Allocated:: %i, at %i ptrs\n",total_memory_allocated,index);
    // Topped out so now we start removing and see what happens
    for (int i=0; i < index; i++){
      if (my_free(arr[i])){
        printf("Successfully removed  %u\n",(unsigned int*)arr[i]);
      }else {
        printf("Failed to remove %u\n",(unsigned int*)arr[i]);
      }
    }
    ptr = my_malloc(sizeof(int));
    index =0;
    total_memory_allocated = 0;
    while(ptr != NULL){
      *(int*)ptr = index;
      total_memory_allocated += sizeof(int);
      arr[index] = ptr;
      index += 1;
      ptr = my_malloc(sizeof(int));
    }
    printf("Total Memory Allocated:: %i, at %i ptrs\n",total_memory_allocated,index); 
  }else{
    printf("Allocation Failed\n");
  }
}

/*--------------------------------------------------------------------------*/
/* UTILITY FUNCTIONS */
/*--------------------------------------------------------------------------*/

void print_time_diff(struct timeval * tp1, struct timeval * tp2) {
  /* Prints to stdout the difference, in seconds and museconds, between
     two timevals. 
  */

  long sec = tp2->tv_sec - tp1->tv_sec;
  long musec = tp2->tv_usec - tp1->tv_usec;
  if (musec < 0) {
    musec += 1000000;
    sec--;
  }
  printf(" [sec = %ld, musec = %ld] ", sec, musec);

}


