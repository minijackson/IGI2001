%:include <stdio.h>
%:include <string.h>
%:include <stdlib.h>

%:ifndef ELEVE_TAB_SIZE
%:define ELEVE_TAB_SIZE 5
%:endif

%:ifndef NAME_BUF_SIZE
%:define NAME_BUF_SIZE 20
%:endif

struct eleve <%
	char nom<:NAME_BUF_SIZE:>;
	char prenom<:NAME_BUF_SIZE:>;
	unsigned int age;
	unsigned int notes<:10:>;
	// Necessary as sizeof won't work through functions
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

void printEleveTable(struct eleve* tab, unsigned int tabSize) <%
	printf("  +--------------+---------------+-------+---------------------\n");
	printf("  | NOM  \t | PRÉNOM\t | AGE\t | NOTES\n");
	printf("  +--------------+---------------+-------+---------------------\n");

	unsigned int i;
	for (i = 0 ; i < tabSize ; i++) <%
		printEleve(tab<:i:>);
	%>

	printf("  +--------------+---------------+-------+---------------------\n");
%>

int main () <%
	struct eleve EleveTab<::> = <%
		<%"durand"    , "leon" , 20 , <%11 , 13 , 18      %> , 3  %> ,
		<%"lake"      , "lea"  , 24 , <%16 , 17 , 19 , 12 %> , 4  %> ,
		<%"pouce"     , "tom"  , 26 , <%11 , 12           %> , 2  %> ,
		<%"Dinoso"    , "tony" , 19 , <%15 , 15 , 15      %> , 3  %> ,
		<%"skywalker" , "luc"  , 23 , <%11 , 11 , 10      %> , 3  %>
	%>;

	struct eleve sortedEleveTab<:ELEVE_TAB_SIZE:>;

	char elevesNoms<:ELEVE_TAB_SIZE:><:NAME_BUF_SIZE:> = <%
		"durand",
		"lake",
		"pouce",
		"Dinoso",
		"skywalker"
	%>;

	qsort(elevesNoms, ELEVE_TAB_SIZE, NAME_BUF_SIZE, (__compar_fn_t)strcmp);

	/* For each name in the sorted name list, find its structure in the
	   EleveTab variable and put it in the sortedEleveTab variable */
	unsigned int i;
	for (i = 0; i < ELEVE_TAB_SIZE; i++) <%
		int j;
		for (j = 0; j < ELEVE_TAB_SIZE; j++) <%
			if(strcmp(elevesNoms<:i:>, EleveTab<:j:>.nom) == 0) <%
				sortedEleveTab<:i:> = EleveTab<:j:>;
				break;
			%>
		%>
	%>

	printEleveTable(EleveTab, ELEVE_TAB_SIZE);
	printf("\n============================================================================\n\n");
	printEleveTable(sortedEleveTab, ELEVE_TAB_SIZE);

	return 0;
%>
