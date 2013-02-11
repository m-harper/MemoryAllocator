#include <stdio.h>
#include "my_allocator.h"
#include "test.h"

int mem_block_size = 4096;

int main(int argc, char **argv) {
	// Allocate some memory for the testing
	void* free_memory = malloc(sizeof(Header) + mem_block_size);
	
	// Place a header on the free memory
	Header* header = free_memory;
	init_Header(header);
	header->size = basic_logarithm(mem_block_size);
	
	if (basic_header_test(header)) 
		printf("Failed basic header test\n");

	if (split_block_test(header)) 
		printf("Failed split block test\n");

	if (list_test())
		printf("Failed list test\n");
		
	if (malloc_test())
		printf("Malloc test failed\n");
		
	printf("Smallest power of two above 2147483648: %lu\n", smallest_power_of_two_above(2147483648));
	
	
	// Free the memory used for testing
	free(free_memory);
	
	return 0;
}

