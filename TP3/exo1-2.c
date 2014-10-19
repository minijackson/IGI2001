#include <stdio.h>

int main (int argc, char const* argv[]) {
	char string[] = "Hello World\n";
	int i;
	
	// Print the entire string
	printf("%s", string);

	// Go through the string until the end ('\0')
	for(i = 0 ; string[i] != '\0' ; ++i) {
		// Print the current char
		printf("%c", string[i]);
	}

	return 0;
}
