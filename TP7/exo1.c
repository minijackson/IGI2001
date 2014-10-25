#include <stdio.h>
#include <string.h>

int main() {
	char nom[20] = "", prenom[20];
	int age;
	FILE* f = fopen("exo1.f.out", "w");

	if(f == NULL) {
		printf("Could not open file\n");
		return 1;
	}

	while(strcmp(nom, "FIN") != 0) {
		printf("Quel est votre nom ?\n");
		scanf("%s", nom);
		if(strcmp(nom, "FIN") == 0)
			break;
		printf("Quel est votre prénom ?\n");
		scanf("%s", prenom);
		printf("Quel est votre age ?\n");
		scanf("%i", &age);

		fprintf(f, "Nom: %s , Prénom: %s , Age: %i\n", nom, prenom, age);

	}

	fclose(f);

	return 0;
}
