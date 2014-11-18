#include <stdio.h>
#include "lib/utils.h"

int main() {
	FILE* f = fopen("hist.txt", "w");
	if(f == NULL)
		return 1;

	size_t* hist = histogram("dict.txt");
	for(unsigned int i = 1 ; i < hist[0] ; ++i) {
		fprintf(f, "%u\t%lu\n", i, hist[i]);
	}
	return 0;
}
