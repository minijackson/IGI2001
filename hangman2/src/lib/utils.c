#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>

#include "utils.h"

unsigned int countWords(const char* const filename) {
	FILE* f = fopen(filename, "r");

	if(f == NULL) {
		fprintf(stderr, "Cannot open file '%s' for counting words.\n", filename);
		exit(EX_NOINPUT);
	}

	unsigned int n = 0;
	char word[30];
	while(!feof(f)) {
		fscanf(f, "%s", word);
		++n;
	}

	// Count two times the last word
	return n - 1;
}

unsigned int ranInt(const unsigned int n) {
	if(n == 0) {
		fprintf(stderr, "Invalid argument for random integer: expected greater than 1 value\n");
		exit(EX_USAGE);
	}

	return (rand() % n) + 1;
}

char* searchWord(const char* const filename, const unsigned int n) {
	FILE* f = fopen(filename, "r");

	if(f == NULL)
		return NULL;

	// Static because used only once in this program
	static char re[30];

	for(unsigned int i = 0 ; i < n ; ++i)
		fscanf(f, "%s", re);

	return re;
}

char* ranWord(const char* const filename, char* const word) {
	char* const re = searchWord(filename, ranInt(countWords(filename)));
	strcpy(word, re);
	free(re);
	return word;
}
