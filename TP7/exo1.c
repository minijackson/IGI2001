#include <stdio.h>
#include <string.h>

int main() {
	char nom[20] = "", prenom[20];
	int age;
	FILE* f = fopen("exo1.f.out", "w");

	while(strcmp(nom, "FIN") != 0) {
		printf("Quel est votre nom ?\n");
		scanf("%19s", nom);
		if(strcmp(nom, "FIN") == 0)
			break;
		printf("Quel est votre prénom ?\n");
		scanf("%19s", prenom);
		printf("Quel est votre age ?\n");
		scanf("%i", &age);

		fprintf(f, "Nom: %s , Prénom: %s , Age: %i\n", nom, prenom, age);

	}

	fclose(f);

	return 0;
}
