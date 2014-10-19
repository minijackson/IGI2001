#include <stdio.h>

void affiche_bin(short x)
{
	int i, m;
	unsigned short sz = sizeof(x) * 8;
	for (i = 0, m = 0x8000; i < sz; ++i, m >>= 1) {
		printf("%i", (x & m) >> ((sz-1)-i));
	}
	printf("\n");
}

int main () {
	unsigned short x = 0b0000111000101100,
				   masque1 = 0b1111111100000000,
				   masque2 = 0b0000000011111111,
				   poidsfort = x & masque1,
				   poidsfaible = x & masque2;

	// %x allows hexadecimal representation
	// But there is no binary representation conversion specifier
	printf("masque 1 : %d => %x\n", masque1, masque1);
	printf("masque 2 : %d => %x\n", masque2, masque2);
	printf("Poids fort de x : %d => %x\n", poidsfort, poidsfort);
	printf("Poids faible de x : %d => %x\n", poidsfaible, poidsfaible);
	affiche_bin(x);

	return 0;
}
