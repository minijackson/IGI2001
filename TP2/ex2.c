#include<stdio.h>

int main (int argc, char const* argv[]) {
	int compteur=0, somme=0;

	do {
		compteur++;
		somme=somme+compteur;
	} while (somme < 10);

	printf("compteur=%d, somme=%d \n", compteur, somme);

	return 0;
}
