#include <stdio.h>
#include <stdlib.h>

unsigned long long int factoriel(int n) {
	if (n <= 1) {
		return 1;
	} else {
		unsigned long long int temp = 0;
		printf("Avant fact: temp=%llu, n=%i\n", temp, n);
		temp = factoriel(n-1) * n;
		printf("Après fact: temp=%llu, n=%i\n", temp, n);
		return temp;
	}
}

int main (int argc, char const* argv[]) {
	if (argc != 2) {
		printf("Usage: %s n\n", argv[0]);
	} else {
		printf("%llu\n",factoriel(atoi(argv[1])));
	}
	return 0;
}
