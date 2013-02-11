#include "test.h"

status basic_header_test(Header* _header) {
	Header* header = _header;
	
	// Fill a header with arbitrary data
	if ( ! header->is_allocated) {
		int i;
		byte* data;
		for (i = 0; i < get_free_size(header); i++) {
			data = (byte*) ((long) get_free_space(_header) + i);
			// Write every bit to a 1
			*data = 255;
		}
		header->is_allocated = true;
	}
	
	unsigned max_num = *(unsigned*) get_free_space(header);

	if (max_num != (unsigned) pow(2, 32))
		return FAILED;
	
	header->is_allocated = false;
	return PASSED;
}

status split_block_test(Header* _header) {
	Header* header = _header;
	
	// Initialize the free list
	init_freelist();
	add_header(header);
	
	// Store the size before the split
	short size = header->size;
	
	Header* split = split_block(header);

	
	if (split == NULL) 
		return FAILED;
	else {
		// Remove the old header from free list
		remove_header(_header);
		// Add the two new headers after correcting sizes
		_header->size -= 1;
		split->size = _header->size - 1;
		add_header(_header);
		add_header(split);
	}
	
	// Check to see if the blocks ended up in the correct locations
	if (freelist[size - 1][0] != _header)
		return FAILED;
	if (freelist[size - 2][0] != split)
		return FAILED;	
	
	
	return PASSED;
}

void print_list() {
	Header* head = freelist[5][0];
	while (head != NULL) {
		printf("%lu\t", head);
		head = head->next_header;
	}
	printf("\n");
}

status list_test() {
	// Initialize the lists
	init_freelist();
	
	// Check all are initially null
	int i;
	for (i = 0; i < 32; i++)
		if (freelist[i][0] != NULL || freelist[i][1] != NULL)
			return FAILED;
	
	Header* headers = malloc(4 * sizeof(Header));
	Header* h1 = &headers[0];
	Header* h2 = &headers[1];
	Header* h3 = &headers[2];
	Header* h4 = &headers[3];
	
	h1->size = 5;
	add_header(h1);
	
	// h1
	if (freelist[5][0] != h1 || freelist[5][1] != h1)
		return FAILED;
	
	h2->size = 5;
	add_header(h2);
	
	// h1 h2	
	if (freelist[5][0] != h1 || freelist[5][1] != h2)
		return FAILED;
			
	h3->size = 5;
	add_header(h3);
	
	// h1 h2 h3
	if (freelist[5][0] != h1 || freelist[5][1] != h3)
		return FAILED;
		
	remove_header(h2);
	
	// h1 h3
	if (freelist[5][0] != h1 || freelist[5][1] != h3)
		return FAILED;
		
	h4->size = 5;
	add_header(h4);
	
	// h1 h3 h4	
	if (freelist[5][0] != h1 || freelist[5][1] != h4)
		return FAILED;
		
	remove_header(h1);

	// h3 h4	
	if (freelist[5][0] != h3 || freelist[5][1] != h4)
		return FAILED;
		
	free(headers);
	return PASSED;
}

status malloc_test() {
	int test_size = 4096;
	
	// Store where the original space is
	void* original_space = allocated_space;
	
	init_freelist();
	allocated_space = malloc(sizeof(Header) + test_size);
	Header* header = allocated_space;
	init_Header(header);
	header->size = basic_logarithm(test_size);
	
	add_header(header);
	
	void* data = my_malloc(4095);
	// Fill up the space
	int i;
	byte* index;
	for (i = 0; i < 4095; i++) {
		index = (byte*) (long) data + i;
		*index = 255;
	}
	unsigned max_num = *(unsigned*) data;
	if (max_num != (unsigned) pow(2, 32))
		return FAILED;
	my_free(data);
	
	data = my_malloc(2045);
	for (i = 0; i < 2045; i++) {
		index = (byte*) (long) data + i;
		*index = 255;
	}
	max_num = *(unsigned*) data;
	if (max_num != (unsigned) pow(2, 32))
		return FAILED;
		
	void* block2 = my_malloc(1023);
	
	my_free(block2);
	
	my_free(data);
	
	// List free blocks
	Header* freeblock = freelist[0][0];
	for (i = 0; i < 32; i++) {
		freeblock = freelist[i][0];
		while (freeblock != NULL) {
			printf("%u ", i);
			freeblock = freeblock->next_header;
		}
	}	
	printf("\n");
	
	// Free the space we allocated
	free(allocated_space);
	// Revert space to original
	allocated_space = original_space;
	return PASSED;
}
