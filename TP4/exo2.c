%:include <stdio.h>

#ifndef ELEVE_TAB_SIZE
#define ELEVE_TAB_SIZE 5
#endif

struct eleve <%
	char nom<:20:>;
	char prenom<:20:>;
	unsigned int age;
	unsigned int notes<:10:>;
	unsigned int noteCount;
%>;

void printEleve(struct eleve tab) <%
	unsigned int i;
	printf("  | %s\t | %s  \t | %u\t | ", tab.nom, tab.prenom, tab.age);

	for (i = 0; i < tab.noteCount; i++) <%
		printf("\t%u", tab.notes<:i:>);
	%>

	printf("\n");
%>

int main () <%
	struct eleve EleveTab<::> = <%
		<%"durand"    , "leon" , 20 , <%11 , 13 , 18      %> , 3  %> ,
		<%"lake"      , "lea"  , 24 , <%16 , 17 , 19 , 12 %> , 4  %> ,
		<%"pouce"     , "tom"  , 26 , <%11 , 12           %> , 2  %> ,
		<%"Dinoso"    , "tony" , 19 , <%15 , 15 , 15      %> , 3  %> ,
		<%"skywalker" , "luc"  , 23 , <%11 , 11 , 10      %> , 3  %>
	%>;

	printf("  +--------------+---------------+-------+---------------------\n");
	printf("  | NOM  \t | PRÉNOM\t | AGE\t | NOTES\n");
	printf("  +--------------+---------------+-------+---------------------\n");

	unsigned int i;
	for (i = 0 ; i < ELEVE_TAB_SIZE ; i++) <%
		printEleve(EleveTab<:i:>);
	%>

	printf("  +--------------+---------------+-------+---------------------\n");

	return 0;
%>
