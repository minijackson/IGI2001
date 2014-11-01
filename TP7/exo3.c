#include <stdio.h>

int main() {
	FILE* f = fopen("exo1.f.out", "r");
	char c = 0;

	if(f == NULL) {
		printf("Could not open file\n");
		return 1;
	}

	do {
		c = fgetc(f);
		printf("%c, %i\n", c, c);
	} while(c != EOF);

	fclose(f);

	return 0;
}
