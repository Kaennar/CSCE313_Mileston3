/* 
    Title :: my_allocator.c
    Author:: Nicholas Hemstreet
    Brief :: Implementation of my_alloc.h
*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include "my_allocator.h"
#include "free_list.h"
// Pragmas
#define UL unsigned long
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef struct block_list_type{
  int eff_block_size;
  int max_block_size;
  int num_levels;
  FL_HEADER_TYPE** lvls;
}block_list;


// LOCAL VARIABLES
bool initialized = false;

block_list bl= {0,0,0,NULL};


/**
 * length = total size of the memory allocation
 *
 * basic_block_size = the minimum size of each block
 *
 *
 */
unsigned int init_allocator(unsigned int basic_block_size, unsigned int length){
  // Only initialize once
  if (!initialized){
    initialized = true;
    int eff_block_size = basic_block_size + sizeof(FL_HEADER_TYPE);
    int number_levels = 0;
    int num = length;
    int level_size = eff_block_size;
    while(num/level_size){
      number_levels += 1;
      level_size = 2*level_size;
    }
    level_size = level_size/2;
    // Now we need to setup the block_list object
    FL_HEADER_TYPE** blocklvl = (FL_HEADER_TYPE**)malloc(sizeof(FL_HEADER_TYPE*)*number_levels);
    for (int i=0; i < number_levels; i++){
      blocklvl[i] = NULL;
    }
    bl.eff_block_size = eff_block_size;
    bl.max_block_size = level_size;
    bl.num_levels = number_levels;
    bl.lvls = blocklvl;
    // Create the largest available chunk and set that as the first header
    FL_HEADER_TYPE* first_block = (FL_HEADER_TYPE*)malloc(sizeof(char)*level_size); 
    first_block->next = NULL;
    first_block->prev = NULL;
    first_block->level = number_levels-1;
    bl.lvls[number_levels-1] = first_block;
    
    return 1;
  }else{
    return 0;
  }
}

int release_allocator(){
  if (initialized){
    // Need to recursively delete down the tree
    initialized = false;
  }
}

void print_list(){
  for (int i=0; i<bl.num_levels; i++){
    printf("-------------------\n");
    printf("Level %i \n",i);
    FL_HEADER_TYPE* node = bl.lvls[i];
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
}
/**
 * Coalesce 
 */
int buildup_from_level(int level){
  if(level >= bl.num_levels-1){
    return 0;
  }
  // If this one is not NULL we move on
  if (bl.lvls[level] == NULL){
    return buildup_from_level(level+1);
  }else {
    // Check and see if this level can be coalesced
    FL_HEADER_TYPE* node = bl.lvls[level];
    // Calculate what the range should be
    int blkSze = bl.eff_block_size*(int)pow(2,level);
    while (node != NULL){
      FL_HEADER_TYPE* buddy = node->next;
      while (buddy != NULL){
        // While there might be buddys out there
        if ((int)((UL)buddy - (UL)node) <= blkSze){
          // Found some buddys, time to add them together
          bl.lvls[level] = remove_free_list(bl.lvls[level],node);
          bl.lvls[level] = remove_free_list(bl.lvls[level],buddy);
          FL_HEADER_TYPE* block1 = node;
          block1->level = level+1;
          bl.lvls[level+1] = add_free_list(bl.lvls[level+1],block1); 
          return buildup_from_level(level+1);
        }
        buddy = buddy->next;
      }
      node = node->next; 
    }
    return 1;
  }

}

/**
 * Moves up from the specified level, breaking down the higher levels to lower ones
 * for allocation
 */
int breakdown_to_level(int level){
  if (level ==0 || level >= bl.num_levels){
    return 0;
  }
  int success = 0;
  if (bl.lvls[level] == NULL){
    // Move onward!
    success = breakdown_to_level(level+1);  
  }
  if (bl.lvls[level] != NULL){
    int levelSize = bl.eff_block_size*(int)pow(2,level);
    FL_HEADER_TYPE* node = bl.lvls[level];
    bl.lvls[level] = remove_free_list(bl.lvls[level],node); 
    FL_HEADER_TYPE* block1 = node;
    block1->level = level-1;
    FL_HEADER_TYPE* block2 = (FL_HEADER_TYPE*)((char*)node + levelSize/2 );
    block2->level = level-1;
    bl.lvls[level-1] = add_free_list(bl.lvls[level-1],block1); 
    bl.lvls[level-1] = add_free_list(bl.lvls[level-1],block2); 
    // We've broken down the block we need so we return and move on
    success =  1; 
  }
  return success;
}


/**
 * Gets a pointer for the level
 */
void* get_ptr_for_level(int level){
  if (bl.lvls[level] == NULL){
    int success = breakdown_to_level(level+1);
    if (success){
      // Get the stuff man
      FL_HEADER_TYPE* node = bl.lvls[level];
      bl.lvls[level] = remove_free_list(bl.lvls[level],node);
      // Get the pointer
      void* ptr = (void*)((char*)node + sizeof(FL_HEADER_TYPE));
      return ptr;
    }
  }else{
      FL_HEADER_TYPE* node = bl.lvls[level];
      bl.lvls[level] = remove_free_list(bl.lvls[level],node);
      // Get the pointer
      void* ptr = (void*)((char*)node + sizeof(FL_HEADER_TYPE));
      return ptr;
    
  }
  return NULL; 
}


Addr my_malloc(int length) {
  //print_list();
  if (initialized){
    for (int i=0; i < bl.num_levels; i++){
      int lvlSize = bl.eff_block_size*(int)pow(2,i)- sizeof(FL_HEADER_TYPE);
      if (lvlSize > length){
        void* ptr = get_ptr_for_level(i);
        return ptr;
      }
    }
  }
  return NULL;
}

int my_free(Addr a) {
  if (initialized){
    // Return to the FL_HEADER
    FL_HEADER_TYPE* node = (FL_HEADER_TYPE*)((char*)a - sizeof(FL_HEADER_TYPE));
    printf("Adding node back to level %i\n", node->level);  
    bl.lvls[node->level] = add_free_list(bl.lvls[node->level],node);
    buildup_from_level(0);
    print_list();
    return 1;
  }else{
    return 0;
  }
}

