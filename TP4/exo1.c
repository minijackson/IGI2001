%:include <stdio.h>
%:include <string.h>

int main () <%
	char chaine1<::> = "Bonjour ",
	     chaine2<::> = ", ceci est une chaîne",
		 chaine3<:100:> = "",
		 chaine4<:20:>  = "";

	printf("%s\n", chaine1);
	printf("%s\n", chaine2);

	fgets(chaine4, 20, stdin);

	char* tok = strtok(chaine4, "\n");

	if(strcmp(chaine4, "toto") == 0) <%
		printf("Nein!\n");
	%> else <%
		strcat(chaine3, chaine1);
		strcat(chaine3, tok);
		strcat(chaine3, chaine2);

		printf("%s\n", chaine3);
	%>

	return 0;
%>
