#include <stdio.h>

#ifndef N
#define N 15
#endif

int main (int argc, char const* argv[]) {
	int var1 = 10, i;
	int tab1[N] , tab2[N];
	
	for (i = 0; i < N; i++) {
		tab1[i] = i;
		tab2[i] = tab1[i];
		printf("%u ", tab2[i]);
	}

	printf("\nTaille tab1 : %lu\nTaille tab2 : %lu\n",
			sizeof(tab1), sizeof(tab2));

	printf("Adresse var1 : %p\n", &var1);

	for (i = 0; i < N; i++) {
		printf("Adresse tab1[%u] : %p\n", i, &tab1[i]);
		printf("Adresse tab2[%u] : %p\n", i, &tab2[i]);
	}

	return 0;
}
