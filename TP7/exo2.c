#include <stdio.h>

int main () {
	FILE* f = fopen("exo1.f.out", "r");
	char nom[20], prenom[20];
	int age;

	while(fscanf(f, "Nom: %s , Prénom: %s , Age: %i\n", nom, prenom, &age) == 3)
		printf("Nom:\t%s\nPrenom:\t%s\nAge:\t%i\n", nom, prenom, age);

	fclose(f);
	return 0;
}
