#include <stdio.h>

int main () {
	int tab[100], i;
	for (i = 0; i < 100; ++i) {
		tab[i] = i;
		printf("%i ", tab[i]);
	}
	printf("\n");
	return 0;
}
