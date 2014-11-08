#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#ifndef DEFAULT_WIDTH
//! @brief The width used if "--random" is given in the program parameters
#define DEFAULT_WIDTH 200
#endif

#ifndef DEFAULT_HEIGHT
//! @brief The height used if "--random" is given in the program parameters
#define DEFAULT_HEIGHT 200
#endif

//! @brief 16 millions colors in the RRGGBB color space
#define COLOR_COUNT 16777216
#define THREADS_COUNT 4

enum programMode {
	random,
	bmp,
	rle
};

typedef unsigned char byte;

//! @brief A structure containing the colors of a pixel
struct pixel {
	//! The 3 bytes of colors of the pixel (in the order of the BMP file)
	byte pixelBytes[4];
};

struct ThreadData {
	struct pixel* tab;
	unsigned int tabWidth;
	unsigned int tabHeight;
	unsigned int xStart;
	unsigned int xEnd;
	unsigned int yStart;
	unsigned int yEnd;
	byte toroidal;
	unsigned int red;
	unsigned int green;
	unsigned int blue;
};

//! @brief A structure containing an allocated color
struct colorAllocation {
	XColor color;	//!< The allocated color
	byte allocated;	//!< Equals to 1 if the color has been allocated, 0 else
};

//! The allocated X11 colors
struct colorAllocation allocatedColors[COLOR_COUNT];

// Print Usage <<<
/*! @brief Print the command line arguments for this program
 *
 * @param programName the name used to invoke this program
 */
void printUsage(const char* programName) {
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "   %s --random [width height] [--toroidal]\n", programName);
	fprintf(stderr, "   %s --bmp file.bmp [--toroidal]\n", programName);
	fprintf(stderr, "   %s --rle file.rle [--toroidal]\n", programName);
}
// >>>
// Get Hex Code <<<
/*! @brief Return the hexadecimal code of a pixel
 *
 * It returns the hexadecimal code (RRGGBB) of the given pixel.
 *
 * @param p the pixel from which the color is wanted
 * @param red the position of the red color code
 * @param green the position of the green color code
 * @param blue the position of the blue color code
 * @return The hexadecimal code of the color
 */
char* getHexCode(const struct pixel p, const unsigned short int red,
				const unsigned short int green, const unsigned short int blue) {
	static char colorHexCode[8];
	char redHexCode[3], greenHexCode[3], blueHexCode[3];
	if(p.pixelBytes[red] < 0x10)
		sprintf(redHexCode, "0%X", p.pixelBytes[red]);
	else
		sprintf(redHexCode, "%X", p.pixelBytes[red]);

	if(p.pixelBytes[green] < 0x10)
		sprintf(greenHexCode, "0%X", p.pixelBytes[green]);
	else
		sprintf(greenHexCode, "%X", p.pixelBytes[green]);

	if(p.pixelBytes[blue] < 0x10)
		sprintf(blueHexCode, "0%X", p.pixelBytes[blue]);
	else
		sprintf(blueHexCode, "%X", p.pixelBytes[blue]);

	sprintf(colorHexCode, "#%s%s%s", redHexCode, greenHexCode, blueHexCode);
	return colorHexCode;
}
// >>>
// Get Int Code <<<
/*! @brief Return the int code of a pixel
 *
 * It returns the int code (RRGGBB convert from hexadecimal to decimal)
 * of the given pixel.
 *
 * @param p the pixel from which the color is wanted
 * @param red the position of the red color code
 * @param green the position of the green color code
 * @param blue the position of the blue color code
 * @return The int code of the color
 */
unsigned int getIntCode(const struct pixel p, const unsigned short int red,
						const unsigned short int green,
						const unsigned short int blue) {
	// $intCode = red\times 256^2 + green \times 256 + blue$
	return p.pixelBytes[red]*65536 + p.pixelBytes[green]*256
			+ p.pixelBytes[blue];
}
// >>>
// Is RLE Data <<<
/*! @brief returns 1 if the given character is an RLE data caracter
 *
 * A character is considered a RLE data character if and only if it is an number
 * or a 'b' or 'o' or a '$'.
 *
 * @param c the character to be checked
 * @return 1 if it is a RLE data character, 0 else
 */
byte isRLEdata(const char c) {
	return ('0' <= c && c <= '9') || c == 'b' || c == 'o' || c == '$';
}
// >>>
// Is Allocated <<<
/*! @brief Return 1 if the color is already allocated, 0 else
 *
 * @param intCode the int code of the color to be checked
 * @return 1 if the color is already allocated, 0 else
 */
byte isAllocated(const unsigned int intCode) {
	return allocatedColors[intCode].allocated == 1;
}
// >>>
// Affiche <<<
/*! @brief Display an array of pixels into an X11 window
 *
 * @param dpy the display used
 * @param w the window to be drawn over
 * @param gc the graphical context to use
 * @param tab the array to draw
 * @param width the width of the array
 * @param height the height of the array
 * @param red the position of the red color code
 * @param green the position of the green color code
 * @param blue the position of the blue color code
 */
void affiche(Display *dpy, Window w, GC gc, const struct pixel* const tab,
			const unsigned int width, const unsigned int height,
			const unsigned short int red, const unsigned short int green,
			const unsigned short int blue) {
	unsigned int i , j ;

	for(i = 0 ; i < width ; i++) {
		for(j = 0 ; j < height ; j++) {
			const struct pixel p = tab[i + (j*width)];

			char* const colorHexCode = getHexCode(p, red, green, blue);
			const int   colorIntCode = getIntCode(p, red, green, blue);
			XColor color;
			const Colormap cmap = DefaultColormap(dpy, 0);
			XParseColor(dpy, cmap, colorHexCode, &color);

			if(!isAllocated(colorIntCode)) {
				XAllocColor(dpy, cmap, &color);
				allocatedColors[colorIntCode].color = color;
				allocatedColors[colorIntCode].allocated = 1;
			} else {
				color = allocatedColors[colorIntCode].color;
			}

			XSetForeground(dpy, gc, color.pixel);
			XDrawPoint(dpy, w, gc, i, j);
		}
	}

	XFlush(dpy);
}
// >>>
// Init Randomly <<<
/*! @brief Randomly initialize an array of pixels
 *
 * A pixel have a probability of $\dfrac{1}{5}$ not to be black.
 *
 * @param tab the array of pixels to be filled
 * @param tabWidth the width of the array
 * @param tabHeight the height of the array
 */
void initRandomly(struct pixel* const tab, const unsigned int tabWidth,
					const unsigned int tabHeight) {
	unsigned int i,j;
	for(i = 0 ; i < tabWidth * tabHeight ; ++i) {
		if(rand() % 5) {
			for(j = 0 ; j <= 3 ; ++j)
				tab[i].pixelBytes[j] = 0;
		} else {
			for(j = 0 ; j <= 3 ; ++j)
				tab[i].pixelBytes[j] = rand() % 256;
		}
	}
}
// >>>
// Init From BMP <<<
/*! @brief Initialize an array of pixel with the pixels of a BMP file
 *
 * The file must already be opened. See https://en.wikipedia.org/wiki/BMP_file
 *
 * @param tab the array of pixels to be filled
 * @param f the opened file containing the BMP data
 * @param pixelStart the start of the pixel array inside the BMP file
 * @param pixelEnd the end of the pixel array inside the BMP file
 * @param bpp the number of bytes per pixels in the BMP file
 * @param paddingSize the size of the padding of the BMP file
 * @param tabWidth the width of the array
 * @param tabHeight the height of the array
 */
void initFromBMP(struct pixel* const tab, FILE* const f,
					const long int pixelStart, const long int pixelEnd,
					const unsigned short int bpp,
					const unsigned short int paddingSize,
					const unsigned int tabWidth, const unsigned int tabHeight) {

	fseek(f, pixelStart, SEEK_SET);

	// BMP starts at bottom left
	unsigned int row = tabHeight - 1, col = 0;
	while (ftell(f) < pixelEnd) {
		// Store the pixel into the tab at the right place
		fread(&tab[col + (row*tabWidth)], bpp, 1, f);

		// Then go until the last column
		col = (col == tabWidth - 1)? 0 : col + 1;
		// And after the last column, go one row up and restart at column 0
		if(col == 0) {
			--row;
			// And skip the padding
			fseek(f, paddingSize, SEEK_CUR);
		}
	}
}
// >>>
// Init From RLE <<<
/*! @brief Initialize an array of pixel with an RLE file
 *
 * The file must already be opened. See http://www.conwaylife.com/wiki/RLE
 *
 * @param tab the array of pixels to be filled
 * @param f the opened file containing the RLE data
 * @param tabWidth the width of the array
 * @param tabHeight the height of the array
 */
void initFromRLE(struct pixel* const tab, FILE* const f,
				const unsigned int tabWidth, const unsigned int tabHeight) {
	struct pixel* pattern;
	const struct pixel black = {{0, 0, 0, 0}};
	unsigned int patternWidth = 0, patternHeight = 0,
				 x = 0, y = 0;

	fseek(f, 0, SEEK_END);
	long int end = ftell(f);
	fseek(f, 0, SEEK_SET);

	do {
		const unsigned int lineBeginPos = ftell(f);
		const char firstLineCharacter = fgetc(f);
		if(firstLineCharacter == '#')
			while(fgetc(f) != '\n');

		// Get the height and the width of the pattern
		if(firstLineCharacter == 'x' && patternWidth == 0 && patternHeight == 0) {
			fscanf(f, "%*[ ]=%*[ ]%u,%*[ ]y%*[ ]=%*[ ]%u",
					&patternWidth, &patternHeight);
			pattern = malloc(patternWidth*patternHeight * sizeof(struct pixel));
			while(fgetc(f) != '\n');
		}

		// Beginning of a RLE data line
		if(isRLEdata(firstLineCharacter)) {

			if(patternWidth == 0 || patternHeight == 0) {
				fprintf(stderr, "No non-null pattern width or height given\n");
				exit(EX_DATAERR);
			} else {
				fseek(f, lineBeginPos, SEEK_SET);
				unsigned int cellCount = 0;
				char c = fgetc(f);

				if(isRLEdata(c)) {
					while('0' <= c && c <= '9') {
						// Convert c to number
						cellCount = (cellCount * 10) + (c - '0');
						c = fgetc(f);
					}
					cellCount = (cellCount == 0)? 1 : cellCount;
					// 'b' means dead cell
					if(c == 'b') {
						for(unsigned int i = 0 ; i < cellCount ; ++i, ++x) {
							pattern[x + (y * patternWidth)] = black;
						}
					// 'o' means alive cell
					} else if(c == 'o') {
						for(unsigned int i = 0 ; i < cellCount ; ++i, ++x) {
							/* pattern[x + (y * patternWidth)] = white; */
							for(unsigned int j = 0 ; j <= 3 ; ++j)
								pattern[x + (y * patternWidth)].pixelBytes[j] = rand() % 256;
						}
					// '\$' means end of line (can take a count before)
					} else if(c == '$') {
						for(unsigned int i = 0 ; i < cellCount ; ++i) {
							// Fill the rest of the line
							while(x < patternWidth) {
								pattern[x + (y * patternWidth)] = black;
								++x;
							}
							++y;
							x = 0;
						}
					} else {
						fprintf(stderr, "Error: expected 'b', 'o' or '$' in RLE file\n");
						exit(EX_DATAERR);
					}
				// '!' means end of data. Everything else can be ignored
				} else if(c == '!') {
					// Fill the rest
					while(y < patternHeight) {
						while(x < patternWidth) {
							pattern[x + (y * patternWidth)] = black;
							++x;
						}
						++y;
						x = 0;
					}
					break;
				}

			}

		}
	} while(ftell(f) != end);

	// Centering of the pattern in the window
	const unsigned int left = (tabWidth / 2) - (patternWidth / 2);
	const unsigned int top = (tabHeight / 2) - (patternHeight / 2);

	for(x = 0 ; x < tabWidth ; ++x) {
		for(y = 0 ; y < tabHeight ; ++y) {
			tab[x + (y * tabWidth)] = black;
		}
	}

	// Copy the pattern in the array
	for(x = left ; x < left + patternWidth ; ++x) {
		for(y = top ; y < top + patternHeight ; ++y) {
			tab[x + (y * tabWidth)] = pattern[x - left + ((y-top) * patternWidth)];
		}
	}

	free(pattern);
}
// >>>
// Color Position <<<
/*! @brief Return the position of a color considering its bitmask
 *
 * @param colorMask the bitmask of the color
 * @return The position of the color
 */
unsigned short int colorPosition(const unsigned int colorMask) {
	// $Position = \log_{256}\left(\dfrac{mask}{255}\right)$
	return log(colorMask/255.)/log(256);
}
// >>>
// Is Alive <<<
/*! @brief Returns 1 if the given pixel is considered "alive"
 *
 * A pixel is considered alive when its color is different from black
 *
 * @param p the pixel to be tested
 * @param red the position of the red color code
 * @param green the position of the green color code
 * @param blue the position of the blue color code
 * @return 1 if the pixel is considered "alive", 0 else
 */
unsigned short int isAlive(const struct pixel p, const unsigned short int red,
							const unsigned short int green,
							const unsigned short int blue) {
	return !(p.pixelBytes[red] == 0 && p.pixelBytes[green] == 0
			&& p.pixelBytes[blue] == 0);
}
// >>>
// Neighbour Count <<<
/*! @brief Count the number of "alive neighbours" surrounding a pixel
 *
 * It counts the number of "cells" considered "alive" of a pixel's 8
 * surrounding locations.
 *
 * @param tab the array of pixels
 * @param x the x location of the pixel
 * @param y the y location of the pixel
 * @param tabWidth the width of the array
 * @param tabHeight the height of the array
 * @param red the position of the red color code
 * @param green the position of the green color code
 * @param blue the position of the blue color code
 * @return The number of neighbours of the given pixel
 */
unsigned short int neighbourCount(const struct pixel* const tab,
									const unsigned int x, const unsigned int y,
									const unsigned int tabWidth,
									const unsigned int tabHeight,
									const byte toroidal,
									const unsigned short int red,
									const unsigned short int green,
									const unsigned short int blue) {
	const unsigned int xStart = (x == 0)? 0 : x - 1,
						yStart = (y == 0)? 0 : y - 1,
						xEnd = (x == tabWidth - 1)? tabWidth - 1 : x + 1,
						yEnd = (y == tabHeight - 1)? tabHeight - 1 : y + 1;
	unsigned short int count = 0;
	for(unsigned int i = xStart ; i <= xEnd ; ++i)
		for(unsigned int j = yStart ; j <= yEnd ; ++j)
			if(isAlive(tab[i + (j * tabWidth)], red, green, blue))
				++count;

	if(toroidal) {
		if(x == 0 || x == tabWidth - 1) {
			const unsigned int wrapCol = (x == 0)? tabWidth - 1 : 0;
			for(unsigned int i = yStart ; i <= yEnd ; ++i)
				if(isAlive(tab[wrapCol + (i * tabWidth)], red, green, blue))
					++count;
		}

		if(y == 0 || y == tabHeight - 1) {
			const unsigned int wrapRow = (y == 0)? tabHeight - 1 : 0;
			for(unsigned int i = xStart ; i <= xEnd ; ++i)
				if(isAlive(tab[i + (wrapRow * tabWidth)], red, green, blue))
					++count;
		}

		if((x == 0 || x == tabWidth - 1) && (y == 0 || y == tabHeight - 1)) {
			const unsigned int wrapCol = (x == 0)? tabWidth - 1 : 0;
			const unsigned int wrapRow = (y == 0)? tabHeight - 1 : 0;
			if(isAlive(tab[wrapCol + (wrapRow * tabWidth)], red, green, blue))
				++count;
		}
	}

	return (isAlive(tab[x+(y*tabWidth)],red,green,blue))? count - 1 : count;
}
// >>>
// Mix Neighbours Colors <<<
/*! @brief Return a color which is a mix of all the neighbours colors
 * of a given pixel
 *
 * It uses the mean of the red, green and blue channels of the colors of the
 * neighbours.
 *
 * @param tab the array of pixels
 * @param x the x location of the pixel
 * @param y the y location of the pixel
 * @param tabWidth the width of the array
 * @param tabHeight the height of the array
 * @param red the position of the red color code
 * @param green the position of the green color code
 * @param blue the position of the blue color code
 * @return The mix of all the neighbours colors
 */
struct pixel mixNeighboursColors(const struct pixel* const tab,
								const unsigned int x, const unsigned int y,
								const unsigned int tabWidth,
								const unsigned int tabHeight,
								const byte toroidal,
								const unsigned short int red,
								const unsigned short int green,
								const unsigned short int blue) {
    const unsigned int xStart = (x == 0)? 0 : x - 1,
                 yStart = (y == 0)? 0 : y - 1,
                   xEnd = (x == tabWidth)? tabWidth : x + 1,
                   yEnd = (y == tabHeight)? tabHeight : y + 1;
	const unsigned short int count = neighbourCount(tab, x, y, tabWidth, tabHeight,
												toroidal, red, green, blue);
	unsigned short int redMean = 0, greenMean = 0, blueMean = 0;

	for(unsigned int i = xStart ; i <= xEnd ; ++i)
		for(unsigned int j = yStart ; j <= yEnd ; ++j)
			if(isAlive(tab[i + (j * tabWidth)], red, green, blue)) {
				redMean   += tab[i + (j * tabWidth)].pixelBytes[red];
				greenMean += tab[i + (j * tabWidth)].pixelBytes[green];
				blueMean  += tab[i + (j * tabWidth)].pixelBytes[blue];
			}

	if(toroidal) {
		if(x == 0 || x == tabWidth - 1) {
			const unsigned int wrapCol = (x == 0)? tabWidth - 1 : 0;
			for(unsigned int i = yStart ; i <= yEnd ; ++i)
				if(isAlive(tab[wrapCol + (i * tabWidth)], red, green, blue)) {
					redMean   += tab[wrapCol + (i * tabWidth)].pixelBytes[red];
					greenMean += tab[wrapCol + (i * tabWidth)].pixelBytes[green];
					blueMean  += tab[wrapCol + (i * tabWidth)].pixelBytes[blue];
				}
		}

		if(y == 0 || y == tabHeight - 1) {
			const unsigned int wrapRow = (y == 0)? tabHeight - 1 : 0;
			for(unsigned int i = xStart ; i <= xEnd ; ++i)
				if(isAlive(tab[i + (wrapRow * tabWidth)], red, green, blue)) {
					redMean   += tab[i + (wrapRow * tabWidth)].pixelBytes[red];
					greenMean += tab[i + (wrapRow * tabWidth)].pixelBytes[green];
					blueMean  += tab[i + (wrapRow * tabWidth)].pixelBytes[blue];
				}
		}

		if((x == 0 || x == tabWidth - 1) && (y == 0 || y == tabHeight - 1)) {
			const unsigned int wrapCol = (x == 0)? tabWidth - 1 : 0;
			const unsigned int wrapRow = (y == 0)? tabHeight - 1 : 0;
			if(isAlive(tab[wrapCol + (wrapRow * tabWidth)], red, green, blue)) {
				redMean   += tab[wrapCol + (wrapRow * tabWidth)].pixelBytes[red];
				greenMean += tab[wrapCol + (wrapRow * tabWidth)].pixelBytes[green];
				blueMean  += tab[wrapCol + (wrapRow * tabWidth)].pixelBytes[blue];
			}
		}
	}

	struct pixel mean;
	mean.pixelBytes[red] = redMean / count;
	mean.pixelBytes[green] = greenMean / count;
	mean.pixelBytes[blue] = blueMean / count;
	return mean;
}
// >>>
// Next Step <<<
/*! @brief Compute the next step of the given array of pixel considering
 * the laws of Conway's Game of Life
 *
 * The rules are:
 * - Any live cell with fewer than two live neighbours dies,
 *   as if caused by under-population.
 * - Any live cell with two or three live neighbours lives on
 *   to the next generation.
 * - Any live cell with more than three live neighbours dies,
 *   as if by overcrowding.
 * - Any dead cell with exactly three live neighbours becomes a live cell,
 *   as if by reproduction.
 *
 * @param tab the array of pixels
 * @param tabWidth the width of the array
 * @param tabHeight the height of the array
 * @param red the position of the red color code
 * @param green the position of the green color code
 * @param blue the position of the blue color code
 */
void* nextStep(struct ThreadData* data) {
	struct pixel* tabTmp = malloc(data->tabWidth * data->tabHeight * sizeof(struct pixel));
	// Tab in which the changes are made before applied
	for(unsigned int i = data->xStart ; i < data->xEnd ; i++) {
		for(unsigned int j = data->yStart ; j < data->yEnd ; j++) {
			unsigned short int neighboursCount = neighbourCount(data->tab, i, j,
															data->tabWidth,
															data->tabHeight,
															data->toroidal,
															data->red,
															data->green,
															data->blue);
			if(isAlive(data->tab[i + (j * data->tabWidth)],
						data->red, data->green, data->blue)) {
				// Loneliness and overcrowding
				if(neighboursCount < 2 || neighboursCount > 3) {
					tabTmp[i + (j * data->tabWidth)].pixelBytes[data->red]   = 0;
					tabTmp[i + (j * data->tabWidth)].pixelBytes[data->green] = 0;
					tabTmp[i + (j * data->tabWidth)].pixelBytes[data->blue]  = 0;
				} else
					tabTmp[i + (j * data->tabWidth)] = data->tab[i + (j * data->tabWidth)];

			} else {
				// Reproduction
				if(neighboursCount == 3)
					tabTmp[i + (j * data->tabWidth)] = mixNeighboursColors(data->tab,
																	i, j,
																data->tabWidth,
																data->tabHeight,
																data->toroidal,
																data->red,
																data->green,
																data->blue);
				else {
					tabTmp[i + (j * data->tabWidth)].pixelBytes[data->red]   = 0;
					tabTmp[i + (j * data->tabWidth)].pixelBytes[data->green] = 0;
					tabTmp[i + (j * data->tabWidth)].pixelBytes[data->blue]  = 0;
				}
			}
		}
	}
	memcpy(data->tab, tabTmp, data->tabWidth*data->tabHeight*sizeof(struct pixel));
	return NULL;
}
// >>>

int main (int argc, char const* argv[]) {

	FILE* f;
	unsigned short int bpp, red = 2, green = 1, blue = 0;
	long int pixelStart, pixelEnd;
	byte toroidal = 0;

	for(int i = 0 ; i < COLOR_COUNT ; ++i)
		allocatedColors[i].allocated = 0;

	srand(time(NULL));

	// ====== Check command-line usage ====== <<<
	for(int i = 0 ; i < argc ; ++i) {
		if(!strcmp(argv[i], "--toroidal")) {
			toroidal = 1;
			--argc;
			break;
		}
	}

	if(argc == 1 || argc > 4) {
		printUsage(argv[0]);
		return EX_USAGE;
	} else if(argc == 2 || argc == 4) {
		if(strcmp(argv[1], "--random")) {
			printUsage(argv[0]);
			return EX_USAGE;
		}
	} else if(argc == 3) {
		// Neither "--bmp" nor "--rle"
		if(strcmp(argv[1], "--bmp") && strcmp(argv[1], "--rle")) {
			printUsage(argv[0]);
			return EX_USAGE;
		}
	}
	// >>>

	enum programMode mode;
	if(!strcmp(argv[1], "--random")) {
		mode = random;
	} else if(!strcmp(argv[1], "--bmp")) {
		mode = bmp;
	} else {
		mode = rle;
	}

	unsigned int width, height;

	if(mode == bmp) {
		// BMP <<<
		f = fopen(argv[2], "rb");

		// ====== Check if readable file ====== <<<
		if(f == NULL) {
			fprintf(stderr, "Error: Could not load the file \"%s\"\n", argv[2]);
			return EX_NOINPUT;
		}
		// >>>
		// ====== BMP file magic check ====== <<<
		// If it really is a BMP, the first two bytes are the ASCII code of "BM"
		char magic[3];
		// Read the first two bytes one time starting from the magic memory block
		fread(&magic, 2, 1, f);
		if (!(magic[0] == 'B' && magic[1] == 'M')) {
			fprintf(stderr, "Error: Not a BMP file\n");
			return EX_DATAERR;
		}
		// >>>
		// ====== Width and Height ====== <<<
		// width and height located at offsets 0x12 and 0x16
		fseek(f, 0x12, SEEK_SET);
		// Load the width and height into the corresponding variables
		// (4 bytes each)
		fread(&width, 4, 1, f);
		fread(&height, 4, 1, f);

		printf("Height = %u, Width = %u\n", height, width);
		// >>>
		// ====== Pixel array offset ====== <<<
		fseek(f, 0xA, SEEK_SET);
		fread(&pixelStart, 4, 1, f);
		// Go to the end
		fseek(f, 0, SEEK_END);
		pixelEnd = ftell(f);
		printf("Pixel array starts at offset: %lX and ends at offset: %lX\n",
				pixelStart, pixelEnd);
		// >>>
		// ====== Bytes per pixels ====== <<<
		// The number of bytes per pixels located at offset 0x1C
		fseek(f, 0x1C, SEEK_SET);
		fread(&bpp, 2, 1, f);
		// Convert to bytes
		bpp /= 8;

		printf("Number of bytes per pixels: %hu\n", bpp);
		if(bpp == 4) {
			fprintf(stderr, "Warning: Alpha channel will not be displayed.\n");
			unsigned int redMask, greenMask, blueMask;
			fseek(f, 0x36, SEEK_SET);
			fread(&redMask, 4, 1, f);
			red = colorPosition(redMask);
			fread(&greenMask, 4, 1, f);
			green = colorPosition(greenMask);
			fread(&blueMask, 4, 1, f);
			blue = colorPosition(blueMask);
		} else if(bpp != 3) {
			fprintf(stderr, "Error: Only RGB and RGBA file supported.\n");
			return EX_DATAERR;
		}
		// >>>
		// >>>
	} else if(mode == random) {
		// Random <<<
		if(argc == 2)
			width = DEFAULT_WIDTH, height = DEFAULT_WIDTH;
		else
			width = atoi(argv[2]), height = atoi(argv[3]);
		// >>>
	} else {
		// RLE <<<
		f = fopen(argv[2], "r");

		// ====== Check if readable file ======
		if(f == NULL) {
			fprintf(stderr, "Error: Could not load the file \"%s\"\n", argv[2]);
			return EX_NOINPUT;
		}

		width = DEFAULT_WIDTH, height = DEFAULT_WIDTH;
		// >>>
	}

	struct pixel* pic = malloc(width*height*sizeof(struct pixel));

	if(mode == bmp) {
		// BMP <<<
		// ====== Padding Size ====== <<<

		// There is a padding column until the row reaches a multiple of 4 bytes
		// $bytesPerRow = rowSize \times bytesPerPixel$
		// $paddingSize = \begin{dcases*}0 & if $bytesPerRow \% 4 = 0$\\4 - (bytesPerRow \% 4) & else\end{dcases*}$
		const unsigned short int paddingSize = (4 - ((width * bpp) % 4) ) % 4;
		// >>>
		// Read pixel array
		initFromBMP(pic, f, pixelStart, pixelEnd, bpp, paddingSize, width, height);
		fclose(f);
		// >>>
	} else if(mode == random) {
		// Random <<<
		initRandomly(pic, width, height);
		// >>>
	} else {
		// RLE <<<
		initFromRLE(pic, f, width, height);
		// >>>
	}

	// ====== X11 initialization ====== <<<
	XEvent e;
	Display* const dpy = XOpenDisplay(NULL);
	const int noir = BlackPixel(dpy, DefaultScreen(dpy));
	const Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
			width, height, 0, noir, noir);
	XMapWindow(dpy, w);
	const GC gc = XCreateGC(dpy,w,0,NULL);
	XSelectInput(dpy, w, StructureNotifyMask);

	while (e.type != MapNotify)
		XNextEvent(dpy, &e);
	// >>>

	affiche(dpy, w, gc, pic, width, height, red, green, blue);

	pthread_t threads[THREADS_COUNT];
	struct ThreadData data[THREADS_COUNT];
	unsigned short int tasksPerThread = ((width)+THREADS_COUNT - 1)/THREADS_COUNT;
	for(;;) {
		sleep(.5);
		for(unsigned int i = 0 ; i < THREADS_COUNT ; ++i) {
			data[i].tab = malloc(width*height*sizeof(struct pixel));
			memcpy(data[i].tab, pic, width*height*sizeof(struct pixel));
			data[i].tabWidth = width;
			data[i].tabHeight = height;
			data[i].xStart = i*tasksPerThread;
			data[i].xEnd = (i+1)*tasksPerThread;
			data[i].yStart = 0;
			data[i].yEnd = height;
			data[i].toroidal = toroidal;
			data[i].red = red;
			data[i].green = green;
			data[i].blue = blue;
		}
		data[THREADS_COUNT - 1].xEnd = width;
		data[THREADS_COUNT - 1].yEnd = height;
		for(unsigned int i = 0 ; i < THREADS_COUNT ; ++i) {
			pthread_create(&threads[i], NULL, nextStep, &data[i]);
		}

		for(unsigned int i = 0 ; i < THREADS_COUNT ; ++i) {
			pthread_join(threads[i], NULL);
		}
		for(unsigned int i = 0 ; i < THREADS_COUNT ; ++i) {
			for(unsigned int j = data->xStart ; j < data->xEnd ; j++)
				for(unsigned int k = data[i].yStart ; k < data[i].yEnd ; k++)
					pic[j + (k*width)] = data[i].tab[j + (k*width)];
		}

		affiche(dpy, w, gc, pic, width, height, red, green, blue);
	}
	sleep(5);

	return 0;
}
// vim: fdm=marker:fmr=<<<,>>>
