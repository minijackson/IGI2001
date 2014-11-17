#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>

#ifdef HAVE__BOOL
#	include <stdbool.h>
#else
#	define bool unsigned char
#	define true 1
#	define false 0
#endif

// Convert a macro to a string containing the macro expansion
#define STR_EXPAND(macro) #macro
#define STR(macro) STR_EXPAND(macro)

#include "lib/utils.h"

struct picture {
	char* str;
	unsigned short int height;
};

bool havePicsFile = false;
char const* const hardcodedPictures[] = {
	"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n",
	"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n      ________________________________\n     /                                /|\n    /                                / |\n   /                                /  |\n  /                                /   |\n /                                /    |\n/________________________________/     |\n|                                |    /\n|                                |   /\n|                                |  /\n|                                | /\n|________________________________|/\n",
	"\n                   __\n                  /  \\\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n      ____________|  |________________\n     /            |  |                /|\n    /             |  |               / |\n   /              |__|              /  |\n  /                                /   |\n /                                /    |\n/________________________________/     |\n|                                |    /\n|                                |   /\n|                                |  /\n|                                | /\n|________________________________|/\n",
	"                    ___________________________________\n                   /  _________________________________)\n                  /  /  / /\n                  |  | / /\n                  |  |/ /\n                  |  | /\n                  |  |/\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n      ____________|  |________________\n     /            |  |                /|\n    /             |  |               / |\n   /              |__|              /  |\n  /                                /   |\n /                                /    |\n/________________________________/     |\n|                                |    /\n|                                |   /\n|                                |  /\n|                                | /\n|________________________________|/\n",
	"                    ___________________________________\n                   /  _________________________) )_____)\n                  /  /  / /                    | |\n                  |  | / /                     | |\n                  |  |/ /                      | |\n                  |  | /                       | |\n                  |  |/                        | |\n                  |  |                      ()(_)()\n                  |  |                    ()       ()\n                  |  |                   ()  X   X ()\n                  |  |                   ()(\\  __  ()\n                  |  |                    ()(\\_____|)\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n                  |  |\n      ____________|  |________________\n     /            |  |                /|\n    /             |  |               / |\n   /              |__|              /  |\n  /                                /   |\n /                                /    |\n/________________________________/     |\n|                                |    /\n|                                |   /\n|                                |  /\n|                                | /\n|________________________________|/\n",
	"                    ___________________________________\n                   /  _________________________) )_____)\n                  /  /  / /                    | |\n                  |  | / /                     | |\n                  |  |/ /                      | |\n                  |  | /                       | |\n                  |  |/                        | |\n                  |  |                      ()(_)()\n                  |  |                    ()       ()\n                  |  |                   ()  X   X ()\n                  |  |                   ()(\\  __  ()\n                  |  |                    ()(\\_____|)\n                  |  |                      __/ _ \\__\n                  |  |                      | \\( )/ |\n                  |  |                      |  \\_/  |\n                  |  |                      |   |   |\n                  |  |                      |   |   |\n                  |  |                      |   |   |\n                  |  |                      |___|___|\n      ____________|  |________________      |_______|\n     /            |  |                /|\n    /             |  |               / |\n   /              |__|              /  |\n  /                                /   |\n /                                /    |\n/________________________________/     |\n|                                |    /\n|                                |   /\n|                                |  /\n|                                | /\n|________________________________|/\n",
	"                    ___________________________________\n                   /  _________________________) )_____)\n                  /  /  / /                    | |\n                  |  | / /                     | |\n                  |  |/ /                      | |\n                  |  | /                       | |\n                  |  |/                        | |\n                  |  |                      ()(_)()\n                  |  |                    ()       ()\n                  |  |                   ()  X   X ()\n                  |  |                   ()(\\  __  ()\n                  |  |                    ()(\\_____|)\n                  |  |                     ___/ _ \\__\n                  |  |                    /   \\( )/ |\n                  |  |                   /  |  \\_/  |\n                  |  |                   |  |   |   |\n                  |  |                   |  |   |   |\n                  |  |                   |  |   |   |\n                  |  |                   |__|___|___|\n      ____________|  |________________   |  |_______|\n     /            |  |                /|  WW\n    /             |  |               / |\n   /              |__|              /  |\n  /                                /   |\n /                                /    |\n/________________________________/     |\n|                                |    /\n|                                |   /\n|                                |  /\n|                                | /\n|________________________________|/\n",
	"                    ___________________________________\n                   /  _________________________) )_____)\n                  /  /  / /                    | |\n                  |  | / /                     | |\n                  |  |/ /                      | |\n                  |  | /                       | |\n                  |  |/                        | |\n                  |  |                      ()(_)()\n                  |  |                    ()       ()\n                  |  |                   ()  X   X ()\n                  |  |                   ()(\\  __  ()\n                  |  |                    ()(\\_____|)\n                  |  |                     ___/ _ \\___\n                  |  |                    /   \\( )/   \\\n                  |  |                   /  |  \\_/  |  \\\n                  |  |                   |  |   |   |  |\n                  |  |                   |  |   |   |  |\n                  |  |                   |  |   |   |  |\n                  |  |                   |__|___|___|__|\n      ____________|  |________________   |  |_______|  |\n     /            |  |                /|  WW         WW\n    /             |  |               / |\n   /              |__|              /  |\n  /                                /   |\n /                                /    |\n/________________________________/     |\n|                                |    /\n|                                |   /\n|                                |  /\n|                                | /\n|________________________________|/\n",
	"                    ___________________________________\n                   /  _________________________) )_____)\n                  /  /  / /                    | |\n                  |  | / /                     | |\n                  |  |/ /                      | |\n                  |  | /                       | |\n                  |  |/                        | |\n                  |  |                      ()(_)()\n                  |  |                    ()       ()\n                  |  |                   ()  X   X ()\n                  |  |                   ()(\\  __  ()\n                  |  |                    ()(\\_____|)\n                  |  |                     ___/ _ \\___\n                  |  |                    /   \\( )/   \\\n                  |  |                   /  |  \\_/  |  \\\n                  |  |                   |  |   |   |  |\n                  |  |                   |  |   |   |  |\n                  |  |                   |  |   |   |  |\n                  |  |                   |__|___|___|__|\n      ____________|  |________________   |  |_______|  |\n     /            |  |                /|  WW|   |    WW\n    /             |  |               / |    |   |\n   /              |__|              /  |    |   |\n  /                                /   |    |   |\n /                                /    |    |   |\n/________________________________/     |    |___|\n|                                |    /     /  /\n|                                |   /     /  /\n|                                |  /      |_/\n|                                | /\n|________________________________|/\n",
	"                    ___________________________________\n                   /  _________________________) )_____)\n                  /  /  / /                    | |\n                  |  | / /       /\\ /\\         | |\n                  |  |/ /          V           | |          /\\ /\\\n                  |  | /                       | |            V\n                  |  |/                        | |\n                  |  |                      ()(_)()\n                  |  |                    ()       ()\n                  |  |     /\\ /\\         ()  X   X ()\n                  |  |       V           ()(\\  __  ()\n                  |  |                    ()(\\_____|)\n                  |  |                     ___/ _ \\___\n                  |  |                    /   \\( )/   \\\n                  |  |                   /  |  \\_/  |  \\\n                  |  |                   |  |   |   |  |\n                  |  |                   |  |   |   |  |\n                  |  |                   |  |   |   |  |\n                  |  |                   |__|___|___|__|\n      ____________|  |________________   |  |_______|  |\n     /            |  |                /|  WW|       |WW\n    /             |  |               / |    |   |   |\n   /              |__|              /  |    |   |   |\n  /                                /   |    |   |   |\n /                                /    |    |   |   |\n/________________________________/     |    |___|___|\n|                                |    /     /  / \\  \\\n|                                |   /     /  /   \\  \\\n|                                |  /      |_/     \\_|\n|                                | /\n|________________________________|/\n",
};

void checkPicsFile() {
	char const* const dirname = STR(DATADIR) "/" PACKAGE;
	struct stat st;
	if(stat(dirname, &st) == 0) {
		for(unsigned int i = 1 ; i <= 10 ; ++i) {
			char filename[255];
			sprintf(filename, "%s/%d.txt", dirname, i);
			// Check if file does not exists
			if(access(filename, F_OK)) {
				havePicsFile = false;
				return;
			}
		}
		havePicsFile = true;
	} else
		havePicsFile = false;
}

struct picture getPicture(short int n) {
	if(havePicsFile) {
		char filename[255];
		sprintf(filename, STR(DATADIR) "/" PACKAGE "/%d.txt", n + 1);
		FILE* f = fopen(filename, "r");

		static char str[2048] = "";
		// Reset the string
		str[0] = '\0';
		char line[65];
		unsigned short int height = 0;
		// Read the file and count the lines
		while(fgets(line, 65, f) != NULL) {
			strcat(str, line);
			++height;
		}

		fclose(f);

		struct picture re = { str, height };
		return re;

	} else {
		struct picture re = { hardcodedPictures[n], 31 };
		return re;
	}
}

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
	printf(" -h,      --help               display this help and exit\n");
	printf(" -v,      --version            display version information and exit\n");
	printf(" -d file, --dictionnary file   use 'file' as dictionnary for random words\n\n");
	printf("Report bugs to <" PACKAGE_BUGREPORT ">.\n");
}

void printVersion() {
	printf(PACKAGE_STRING "\n");
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
	unsigned short int remainingAttempts = 10;

	unsigned int wordsInFile = countWords(wordsFile);
	char* const gameWord = searchWord(wordsFile, ranInt(wordsInFile));
	unsigned short int wordLength = strlen(gameWord);

	char hiddenWord[wordLength + 1];
	for(unsigned short int i = 0 ; i < wordLength ; ++i)
		hiddenWord[i] = '_';
	hiddenWord[wordLength] = '\0';

	char triedLetters[26]  = "                          ";
	unsigned int position = 0;

	bool alreadyTried = false;
	bool invalidCharacter = false;

	checkPicsFile();

	while(!(win || lose)) {

		struct picture pic = getPicture(10 - remainingAttempts);
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
				--remainingAttempts;
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
		else if(remainingAttempts == 0)
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
