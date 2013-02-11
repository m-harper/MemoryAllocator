#include "header.h"

void init_Header(Header* _header) {
		_header->next_header = NULL;
		_header->is_allocated = false;
		_header->size = 0;
}

long get_free_size(Header* _header) {
		return pow(2.0, _header->size);
}

void* get_free_space(Header* _header) {
		void* space = _header;
		int offset = sizeof(Header);
		space = (void*) (long) space + offset;
		return space;
}

Header* split_block(Header* _header) {
	Header* failed = NULL;
	
	if (_header->is_allocated) {
		return failed;
	}
		
	/*if (pow(2.0, _header->size - 2) < sizeof(Header))
		return _header;*/
		
	if (pow(2.0, _header->size - 2) < block_size)
		return _header;
	
	Header* new_header = (Header*) ((long) _header + sizeof(Header) + (get_free_size(_header)/2));
	init_Header(new_header);
	//_header->size--;
	//new_header->size = _header->size - 1;
	
	return new_header;	
}
