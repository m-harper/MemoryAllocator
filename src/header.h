#ifndef _HEADER_H
#define _HEADER_H

#include "bool.h"
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

typedef struct _Header Header;

struct _Header {
	// Pointer to the next header in 
	// the list of the same size
	Header* next_header; 
	
	// Has this block been allocated to the user
	bool is_allocated;
	
	short size;
};

void init_Header(Header*);
long get_free_size(Header*);
void* get_free_space(Header*);
Header* split_block(Header*);
bool coalesce(Header*);

int block_size;
long mem_size;

#endif
