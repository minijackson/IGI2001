#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/utils.h"

int main() {
	srand(time(NULL));

	const char* const word[30];
	printf("%s\n", ranWord("dict.txt", word));
	return 0;
}
