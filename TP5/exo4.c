#include <stdio.h>
#include <stdlib.h>

// Go through the table and remove all non prime multiple of the prime parameter
void removeMultiples(const int prime, int* tab, const int size)
{
	int i;
	// start with prime as index as the table is ordered
	for (i = prime + 1; i < size; i++) {
		if(tab[i] % prime == 0)
			tab[i] = 0;
	}
}

int main (int argc, char const* argv[]) {
	if (argc != 2)
		printf("Usage : %s N\n", argv[0]);
	else {
		int n = atoi(argv[1]), tab[n], i;

		// Generate the table
		for (i = 0; i < n; ++i) {
			tab[i] = i + 1;
		}

		printf("| ");
		for (i = 1; i < n; ++i) {
			if(tab[i] != 0) {
				removeMultiples(tab[i], tab, n);
				printf("%i \t| ", tab[i]);
			}
		}
		printf("\n");
	}
	return 0;
}
