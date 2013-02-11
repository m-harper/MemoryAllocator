#ifndef _ALLOCATOR_TEST_H
#define _ALLOCATOR_TEST_H

#include "bool.h"
#include "header.h"
#include "util.h"
#include "freelist.h"
#include "my_allocator.h"
#include <stdio.h>

typedef enum {PASSED, FAILED} status;

status basic_header_test(Header*);
status split_block_test(Header*);
status list_test();
status malloc_test();

#endif
