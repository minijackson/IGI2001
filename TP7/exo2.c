#include <stdio.h>

int main() {
	FILE* f = fopen("exo1.f.out", "r");

	if(f == NULL) {
		printf("Could not open file\n");
		return 1;
	}

	char nom[20], prenom[20];
	int age;

	while(fscanf(f, "Nom: %s , Prénom: %s , Age: %i\n", nom, prenom, &age) == 3)
		printf("Nom:\t%s\nPrénom:\t%s\nAge:\t%i\n", nom, prenom, age);

	fclose(f);
	return 0;
}
