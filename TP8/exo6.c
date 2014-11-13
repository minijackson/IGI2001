#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lib/utils.h"

#define true 1
#define false 0
#define bool unsigned char

void goUpLine() {
	printf("\033[A\033[2K");
	rewind(stdout);
}

bool validLetter(char const letter) {
	return 'a' <= letter && letter <= 'z';
}

bool containsLetter(char const* const word,
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

	bool win = 0;
	bool lose = 0;
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

	bool alreadyTried = false;
	bool invalidCharacter = false;

	while(!(win || lose)) {
		if(alreadyTried)
			printf("You already tried it!\n");
		else if(invalidCharacter)
			printf("Invalid character\n");
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
			invalidCharacter = false;
		} else if(validLetter(c)) {
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
			invalidCharacter = false;
			alreadyTried = false;
		} else {
			invalidCharacter = true;
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
			if(c == '\n')
				goUpLine();
		}
	}

	printf("You %s!\n", (win) ? "win" : "lose");
	printf("The word was: %s\n", gameWord);

	free(gameWord);

	return 0;
}
