#include "inc.h"
#include "words.h"


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
