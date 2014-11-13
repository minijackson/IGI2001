#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lib/utils.h"

#define true 1
#define false 0

void goUpLine() {
	printf("\033[A\033[2K");
	rewind(stdout);
}

unsigned char containsLetter(char const* const word,
							char const letter,
							unsigned int length) {
	for(unsigned int i = 0 ; i < length ; ++i) {
		if(word[i] == letter)
			return true;
	}
	return false;
}

int main() {
	srand(time(NULL));

	unsigned char win = 0;
	unsigned char lose = 0;
	unsigned short int remainingTries = 10;

	unsigned int wordsInFile = countWords("dict.txt");
	char* const gameWord = searchWord("dict.txt", ranInt(wordsInFile));
	unsigned short int wordLength = strlen(gameWord);

	char hiddenWord[wordLength + 1];
	for(unsigned short int i = 0 ; i < wordLength ; ++i)
		hiddenWord[i] = '_';
	hiddenWord[wordLength] = '\0';

	char triedLetters[26]  = "                          ";
	triedLetters[25] = ' ';
	unsigned int position = 0;

	unsigned char alreadyTried = false;

	while(!(win || lose)) {
		if(alreadyTried)
			printf("You already tried it!\n");
		else
			printf("\n");
		printf("%s\n", hiddenWord);
		printf("Tried letters: %s\n", triedLetters);
		printf("Your try: ");
		char c = getc(stdin);
		// Pick up the \n
		getc(stdin);
		printf("\n");
		if(containsLetter(triedLetters, c, 26)) {
			alreadyTried = true;
		} else {
			if(containsLetter(gameWord, c, wordLength)) {
				for(unsigned short int i = 0 ; i < wordLength ; ++i) {
					if(gameWord[i] == c)
						hiddenWord[i] = c;
				}
			} else {
				--remainingTries;
			}
			triedLetters[position] = c;
			++position;
			alreadyTried = false;
		}
		if(!containsLetter(hiddenWord, '_', wordLength))
			win = 1;
		else if(remainingTries == 0)
			lose = 1;
		else {
			for(unsigned int i = 0 ; i <= 4 ; ++i) {
				goUpLine();
			}
		}
	}

	printf("You %s!\n", (win) ? "win" : "lose");
	printf("The word was: %s\n", gameWord);

	free(gameWord);

	return 0;
}
