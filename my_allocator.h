/* 
  Title :: my_allocator.h
  Author:: Nicholas Hemstreet
  Brief :: defines the allocator, my_malloc, and free functions for use in the memtest file

*/

#ifndef _my_allocator_h_                   // include file only once
#define _my_allocator_h_


#include<stdlib.h>

// No point in doing the typedefine when we can just do the #define :-)!
typedef void * Addr; 


/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/


/**
 * @brief initializes the allocator that allows the use of the my_alloc/my_free functions. Also sets up the free list which manages the memory. THERE IS NO STRONGLY TYPED MEMORY SAFETY. USER IS TASKED WITH THE CREATION OF THIS (suckers...)
 *
 * @param length  total memory length for the allocator to take up
 *
 * @retval 1 if successful or -1 on failure
 */
unsigned int init_allocator( unsigned int length); 

/**
 * @brief releases all of the memory blocks associated with the allocator
 *
 *
 * @retval 1 if successfully released, 0 if failure
 *
 *
 * */
int release_allocator(); 

/**
 * @brief
 *
 * @param length amount of memory to allocate
 * @retval address of the allocated memory
 * */
Addr my_malloc(int length); 

/**
 * @brief frees up the value at address a
 *
 * @param a address to be released
 *
 * @retval 1 if successfully removed, 0 if there is a failure
 * */
int my_free(Addr a); 

#endif 
