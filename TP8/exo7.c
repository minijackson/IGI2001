#include <stdio.h>
#include "lib/utils.h"

int main() {
	size_t* hist = histogram("dict.txt");
	for(unsigned int i = 0 ; i < hist[0] ; ++i) {
		printf("words of %u characters: %lu\n", i, hist[i]);
	}
	return 0;
}
