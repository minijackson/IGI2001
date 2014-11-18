#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>

#include "utils.h"

unsigned int countWords(const char* const filename) {
	FILE* f = fopen(filename, "r");

	if(f == NULL) {
		fprintf(stderr, "Cannot open file for counting words.\n");
		exit(EX_NOINPUT);
	}

	unsigned int n = 0;
	char word[30];
	while(!feof(f)) {
		fscanf(f, "%s", word);
		++n;
	}

	fclose(f);

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

	// Useless but more beautiful with sizeof(char)
	char* const re = malloc(30*sizeof(char));

	for(unsigned int i = 0 ; i < n ; ++i)
		fscanf(f, "%s", re);

	fclose(f);

	return re;
}

char* ranWord(const char* const filename, char* const word) {
	char* const re = searchWord(filename, ranInt(countWords(filename)));
	strcpy(word, re);
	free(re);
	return word;
}

size_t maxSizeInFile(char* filename) {
	FILE* f = fopen(filename, "r");

	if(f == NULL) {
		fprintf(stderr, "Cannot open file in maxSizeInFile.\n");
		exit(EX_NOINPUT);
	}

	size_t max = 0;
	char word[30];
	while(fgets(word, 30, f) != NULL) {
		size_t count = strlen(word);
		if(count > max)
			max = count;
	}

	fclose(f);

	// Remove the UNIX end of line
	return max - 1;
}

size_t* histogram(char* filename) {
	FILE* f = fopen(filename, "r");

	if(f == NULL) {
		fprintf(stderr, "Cannot open file in filename.\n");
		exit(EX_NOINPUT);
	}

	size_t tabSize = maxSizeInFile(filename) + 1;

	size_t* hist = calloc(tabSize, sizeof(size_t));
	char word[30];
	while(fgets(word, 40, f) != NULL) {
		++hist[strlen(word) - 1];
	}

	hist[0] = tabSize;

	fclose(f);
	return hist;
}
