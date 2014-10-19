#include <stdio.h>
#ifndef BUF_SIZE
#define BUF_SIZE 2048
#endif

int main (int argc, char const* argv[]) {
	char concat[BUF_SIZE];
	int c,i,j = 0;

	// Go through every program parameters except the program name
	for (c = 1; c < argc; c++) {
		// Go through the string until the end ('\0')
		for (i = 0; argv[c][i] != '\0' ; ++i) {
			concat[j] = argv[c][i];
			++j;
		}
		// Add spaces between words
		concat[j] = ' ';
		++j;
	}

	// Adding final string end ('\0')
	concat[j] = '\0';

	printf("%s\n", concat);


	return 0;
}
