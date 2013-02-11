#include "util.h"

// Returns the length of a null terminated string
int get_string_size(char* _string) {
	unsigned size = 0;
	
	while (_string[++size] ) ;

	return size;
}

// Converts a number represented by characters to an integer
int char_to_int(char* _digits) {
	unsigned being_built = 0;
	int number_length = get_string_size(_digits);

	int exponent, add_value;
	int index = 0;
	for ( ; number_length > 0; index++) {
		exponent = (int) pow(10.0, number_length - 1);
		add_value = exponent * ((int) _digits[index] - '0');
		being_built += add_value;
		number_length--;
	}

	return being_built;
}

// Returns the smallest power of two that is greater than the parameter
long smallest_power_of_two_above(long _num) {
	assert(_num > 1);

	long total = 2;
	
	while (total < _num)
			total <<= 1;
	return total;
}

// Returns an integer reprentation of log_2(parameter)
int basic_logarithm(long _num) {
	int power = 0;

	while (pow(2, power) < _num) {
		power++;
	}
	
	return power;
}
