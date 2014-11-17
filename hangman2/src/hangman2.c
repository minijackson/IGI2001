#include "utils/inc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sysexits.h>
#include <getopt.h>

#include "utils/dict.h"
#include "utils/words.h"
#include "utils/pics.h"

void printVersion() {
	printf(PACKAGE_STRING "\n");
}

void printHelp(char const* const progName) {
	printf(PACKAGE_STRING "\n");
	printf("Usage: %s [OPTION]\n\n"
	       "Options\n\n"
	       " -h,      --help               display this help and exit\n"
	       " -v,      --version            display version information and exit\n"
	       " -d file, --dictionnary file   use 'file' as dictionnary for random words\n\n"
	       "Report bugs to <" PACKAGE_BUGREPORT ">.\n", progName);
}

int main(int const argc, char ** const argv) {

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

	char triedLetters[26]  = "";
	unsigned int position = 0;

	bool alreadyTried = false;
	bool invalidCharacter = false;

	checkPicsFile();

	while(!(win || lose)) {

		struct picture pic = getPicture(10 - remainingTries);
		fputs(pic.str, stdout);

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
			triedLetters[position + 1] = '\0';
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
			for(unsigned int i = 0 ; i <= 4 + pic.height ; ++i) {
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
