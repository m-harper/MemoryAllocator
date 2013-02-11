#ifndef _FREELIST_H
#define _FREELIST_H

#include "header.h"
#include <assert.h>

Header* freelist[32][2];

void init_freelist();

void add_header(Header*);
void remove_header(Header*);

void add_back(Header*);


#endif
