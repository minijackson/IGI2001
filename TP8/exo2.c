#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

int main() {
	srand(time(NULL));

	for(unsigned int i = 0 ; i <= 42 ; ++i)
		printf("%u\n", ranInt(42));
	return 0;
}
