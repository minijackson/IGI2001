#include <stdio.h>

void add_mul(int x, int y, int* pointeur_add, int* pointeur_mul) {
	printf("pointeur_add=%i, pointeur_mul=%i\n", *pointeur_add, *pointeur_mul);
	*pointeur_add = x+y;
	*pointeur_mul = x*y;
}

int main () {
	int a = 10, b = 20, res_addition = 0, res_multiplication = 0;
	printf("&a=%p, &b=%p\n", &a, &b);
	add_mul(a, b, &res_addition, &res_multiplication);
	printf("a=%i, b=%i, a+b=%i, a*b=%i\n",
			a, b, res_addition, res_multiplication);
	return 0;
}
