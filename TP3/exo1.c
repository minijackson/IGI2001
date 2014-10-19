#include <stdio.h>

int main (int argc, char const* argv[]) {
	char i;
	
	// Go through the uppercase letters of the ASCII table
	for (i = 'A'; i <= 'Z'; i++) {
		printf("%c : %d\n", i, (int)i);
		// Uppercase letter + 32 = lowercase equivalent
		printf("%c : %d\n", i+32, (int)(i+32));
	}
	return 0;
}
