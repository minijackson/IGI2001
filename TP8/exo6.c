#include <stdio.h>
#include <stdlib.h>

int main() {
	const char* const upLine = "\033[A\033[2K";
	rewind(stdout);
	return 0;
}
