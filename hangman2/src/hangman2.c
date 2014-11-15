#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sysexits.h>
#include <getopt.h>

#ifdef HAVE__BOOL
#	include <stdbool.h>
#else
#	define bool unsigned char
#	define true 1
#	define false 0
#endif

#include "lib/utils.h"

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

void printHelp(char const* const progName) {
	printf(PACKAGE_STRING "\n");
	printf("Usage: %s [OPTION]\n\n", progName);
	printf("Options\n\n");
	printf(" -h, --help           display this help and exit\n");
	printf(" -v, --version        display version information and exit\n");
	printf(" --dictionnary=file   use 'file' as dictionnry for random words\n\n");
	printf("Report bugs to <" PACKAGE_BUGREPORT ">.\n\n");
}

void printVersion() {
	printf(PACKAGE_STRING "\n");
}

int main(int const argc, char ** const argv) {
	printf("DATADIR \n");

	char wordsFile[255] = "/usr/share/dict/french";

	static struct option longOptions[] = {
		{"help"       , no_argument      , 0, 'h'}, 
		{"version"    , no_argument      , 0, 'v'}, 
		{"dictionnary", required_argument, 0, 'd'}, 
		{0            , 0                , 0,  0 }
	};

	int option, optionIndex;
	while((option =  getopt_long(argc, argv, "hvd:",
	                             longOptions, &optionIndex))) {
		if(option == -1)
			break;

		switch(option) {
			case 'h':
				printHelp(argv[0]);
				return 0;
			case 'v':
				printVersion();
				return 0;
			case 'd':
				strcpy(wordsFile, optarg);
				break;
			case '?':
			case ':':
				printf("Try `%s --help' for more information.\n", argv[0]);
				return EX_USAGE;
			default:
				abort();
		}
	}

	srand(time(NULL));

	bool win = 0;
	bool lose = 0;
	unsigned short int remainingTries = 10;

	unsigned int wordsInFile = countWords(wordsFile);
	char* const gameWord = searchWord(wordsFile, ranInt(wordsInFile));
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

	char filename[12] = "pics/nn.txt";
	char ch;

	while(!(win || lose)) {
		sprintf(filename, "pics/%d.txt", 11 - remainingTries);
		FILE* f = fopen(filename, "r");

		while((ch = fgetc(f)) != EOF) {
			putchar(ch);
		}

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
			for(unsigned int i = 0 ; i <= 4 + 31 ; ++i) {
				goUpLine();
			}
			if(c == '\n')
				goUpLine();
		}
	}

	printf("You %s!\n", (win) ? "win" : "lose");
	printf("The word was: %s\n", gameWord);

	return 0;
}
