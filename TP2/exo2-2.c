#include <stdio.h>

long long unsigned int factoriel (int n) {
	if (n > 1) {
		return factoriel(n-1) * n;
	} else {
		return 1;
	}
}

int main (int argc, char const* argv[]) {
	int n;
	for (n = 0; n <= 150; n++) {
		printf("%llu\n", factoriel(n));
	}
	return 0;
}
