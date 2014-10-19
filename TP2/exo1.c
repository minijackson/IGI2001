#include <stdio.h>

int main (int argc, char const* argv[]) {
	char plop1 = 'c';
	unsigned char plop2 = 'c';
	short plop3 = 'c';
	unsigned short plop4 = 'c';
	int plop5 = 'c';
	unsigned int plop6 = 'c';
	long plop7 = 'c';
	long long plop8 = 'c';
	float plop9 = 'c';
	double plop10 = 'c';

	printf("char (plop1) : Size %lo : Value %c : In memory %p \n", sizeof(char), plop1, &plop1);
	printf("unsigned char (plop2) : Size %lo : Value %c : In memory %p \n", sizeof(unsigned char), plop2, &plop2);
	printf("short (plop3) : Size %lo : Value %d : In memory %p \n", sizeof(short), plop3, &plop3);
	printf("unsigned short (plop4) : Size %lo : Value %u : In memory %p \n", sizeof(unsigned short), plop4, &plop4);
	printf("int (plop5) : Size %lo : Value %d : In memory %p \n", sizeof(int), plop5, &plop5);
	printf("unsigned int (plop6) : Size %lo : Value %u : In memory %p \n", sizeof(unsigned int), plop6, &plop6);
	printf("long (plop7) : Size %lo : Value %ld : In memory %p \n", sizeof(long), plop7, &plop7);
	printf("long long (plop8) : Size %lo : Value %lld : In memory %p \n", sizeof(long long), plop8, &plop8);
	printf("float (plop9) : Size %lo : Value %f : In memory %p \n", sizeof(float), plop9, &plop9);
	printf("double (plop10) : Size %lo : Value %f : In memory %p \n", sizeof(double), plop10, &plop10);
	return 0;
}
