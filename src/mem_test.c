#include "ackerman.h"
#include "util.h"
#include "my_allocator.h"
#include <unistd.h>


typedef struct _Settings {
	int block;
	long mem;
} Settings;

Settings handle_args(int, char**);

int main(int argc, char ** argv) {
	atexit((void*) release_allocator);
	
	Settings settings = handle_args(argc, argv);
	init_allocator(settings.block, settings.mem);
	ackerman_main();
	return 0;
}

// Handles block and memory size selection
Settings handle_args(int argc, char ** argv) {

	Settings settings;
	// Give settings initial values to compare against
	settings.block = 0;
	settings.mem = 0;

	char option;

	while ((option = getopt(argc, argv, "b:s:")) != -1) {
		switch(option) {
		case 'b':
			settings.block = char_to_int(optarg);
			break;
		case 's':
			settings.mem = char_to_int(optarg);
			break;
		}
	}

	return settings;
}
