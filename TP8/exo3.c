#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int main() {
	const char* const str = "dict.txt";
	char* const first = searchWord(str, 33609);
	char* const second = searchWord(str, 320115);
	char* const third = searchWord(str, 194240);
	char* const fourth = searchWord(str, 211523);
	printf("%s %s %s %s!\n", first, second, third, fourth);
	free(first);
	free(second);
	free(third);
	free(fourth);
	return 0;
}
