#include "freelist.h"

void init_freelist() {
	int i;
	for (i = 0; i < 32; i++) {
		freelist[i][0] = NULL;
		freelist[i][1] = NULL;
	}
}

void add_header(Header* _header) {
	add_back(_header);
}

void remove_header(Header* _header) {
	int index = _header->size;
	
	if (freelist[index][0] == NULL) {
		return;
	}
	
	// Two cases
	// Case one: only element in list
	if (freelist[index][0] == freelist[index][1]) {
		assert(freelist[index][0] == _header);
		freelist[index][0] = NULL;
		freelist[index][1] = NULL;
		_header->next_header = NULL;
	}
	// Case two: multiple elements
	else {
		// Three cases
		// Case one: header is at beginning
		if (freelist[index][0] == _header) {
			freelist[index][0] = _header->next_header;
			_header->next_header = NULL;
			return;
		}
		// Case two: header is end
		else if (freelist[index][1] == _header) {
			// Find the previous element
			Header* prev = freelist[index][0];
			while (prev != NULL) {
				if (prev->next_header == _header) {
					break;
				}
				prev = prev->next_header;
			}
			assert(prev != NULL);
			prev->next_header = NULL;
			freelist[index][1] = prev;
		}
		// Case three: header is in middle some where
		else {
			// Find the previous element
			Header* prev = freelist[index][0];
			while (prev != NULL) {
				if (prev->next_header == _header) {
					break;
				}
				prev = prev->next_header;
			}
			prev->next_header = _header->next_header;
			_header->next_header = NULL;
		}
	}
}

void add_back(Header* _header) {
	int index = _header->size;
	
	// Two cases
	// Case one: no element in the list
	if (freelist[index][0] == NULL) {
		// Tell the header it is the only element
		_header->next_header = NULL;
		
		// Keep track for ourselves
		freelist[index][0] = _header;
		freelist[index][1] = _header;
	}
	// Case two: there is at least one element already
	else {
		// Update the current end
		freelist[index][1]->next_header = _header;
		
		// Set the new end
		freelist[index][1] = _header;
	}
}
