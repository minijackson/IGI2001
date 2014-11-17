#include "config.h"

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

