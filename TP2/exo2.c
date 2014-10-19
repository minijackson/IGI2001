#include <stdio.h>
#include <stdlib.h>

long long unsigned int factoriel (int n)
{
	if (n <= 0) {
		return 1;
	} else {
		return n*factoriel(n-1);
	}
}

int main (int argc, char const* argv[]) {
	int i;
	for (i = 1; i < argc; i++) {
		printf("%d! = %llu\n", atoi(argv[i]), factoriel(atoi(argv[i])));
	}
	return 0;
}
