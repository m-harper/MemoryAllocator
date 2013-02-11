/* 
    File: my_allocator.c

    Author: <your name>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

	int default_block_size = 8; // 8 B
	long default_mem_size = (long) pow(2.0, 19); // 512 kB 

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/


/* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. If an error occurred, 
   it returns 0. 
*/
extern unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length) {
	if (_basic_block_size)
		block_size = _basic_block_size;
	else
		block_size = default_block_size;
		
	if (_length)
		mem_size = _length;
	else
		mem_size = default_mem_size;
		
	
	init_freelist();
	allocated_space = malloc(sizeof(Header) + mem_size);
	Header* header = allocated_space;
	init_Header(header);
	header->size = basic_logarithm(mem_size);
	add_header(header);
	
	return get_free_size(header);
}

/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails.
*/ 
extern int release_allocator() {
	free(allocated_space);
	return 0;
} 

/* Allocate _length number of bytes of free memory and returns the 
   address of the allocated portion. Returns 0 when out of memory. */ 
extern Addr my_malloc(unsigned int _length) {
	// Round up to the size we need
	int needed_size = basic_logarithm(_length);
	
	// Check if there is already a free block of the needed size
	if (freelist[needed_size][0] != NULL) {
		// There is already a block free
		Header* available = freelist[needed_size][0];
		remove_header(available);
		available->is_allocated = true;
		return (void*) get_free_space(available);
	}
	else {
		// There isn't a block free
		// We need to break up a bigger block
		
		// Find the smallest available
		int power = needed_size + 1;
		
		while (power < 32 && freelist[power][0] == NULL) {
			power++;
		}
		if (power == 32) {
			// No block big enough free
			//printf("None big enough\n");
			return NULL;
		}
		assert(freelist[power][0] != NULL);
		Header* header, *split;
		while (freelist[needed_size][0] == NULL) {
			header = freelist[power][0];
			remove_header(header);
			split = split_block(header);
			if (header == split) {
				// Block can't split further
				header->is_allocated = true;
				return get_free_space(header);
			}
			else if (split == NULL) {
				// Split failed
				return NULL;
			}
			else {
				header->size -= 1;
				split->size = header->size - 1;
				add_header(header);
				add_header(split);
			}
			power--;
		}
		// We have a block ready
		Header* ready = freelist[needed_size][0];
		remove_header(ready);
		ready->is_allocated = true;
		return get_free_space(ready);
	}
}

/* Frees the section of physical memory previously allocated 
   using ’my_malloc’. Returns 0 if everything ok. */
extern int my_free(Addr _a) {
	Header* header = (Header*) ((long) _a - sizeof(Header));
	header->is_allocated = false;
	add_header(header);
	coalesce_headers();
	return 0;
}

extern bool coalesce_headers() {
	Header* larger, *smaller;
	bool merged = false;
	int i;
	// Only loop to 31 because loop accesses i+1
	for (i = 0; i < 31; i++) {
		smaller = freelist[i][0];
		larger = freelist[i+1][0];
		
		while (larger != NULL) {
			while (smaller != NULL) {
				// If the blocks are back to back
				if (((long) larger + sizeof(Header) + get_free_size(larger)) == (long) smaller) {
					assert(!larger->is_allocated && !smaller->is_allocated);
					// Blocks are buddies, merge them
					merge_blocks(larger, smaller);
					merged = true;
				}
				smaller = smaller->next_header;
			}
			smaller = freelist[i][0];
			larger = larger->next_header;
		}
	}
	return merged;
}

extern void merge_blocks(Header* _a, Header* _b) {
	assert(_a != _b);
	
	Header* first, *next;
	if (_a > _b) {
		next = _a;
		first = _b;
	}
	else {
		next = _b;
		first = _a;
	}
	// Remove both of the blocks from the free lists
	remove_header(first);
	remove_header(next);
	
	// Merge the blocks
	first->size += 1;
	
	// Add the header back to the free lists
	add_header(first);
}
 

