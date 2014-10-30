#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <math.h>
#include <time.h>

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

typedef unsigned char byte;

//! @brief A structure containing the colors of a pixel
struct pixel {
	byte pixelBytes[4];			//!< The 3 bytes of colors of the pixel (in the order of the BMP file)
};

//! @brief A structure containing an allocated color
struct colorAllocation {
	XColor color;				//!< The allocated color
	byte allocated;				//!< Equals to 1 if the color has been allocated, 0 else
};

struct colorAllocation allocatedColors[COLOR_COUNT]; //!< The allocated X11 colors

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
char* getHexCode(struct pixel p, unsigned short int red,
				unsigned short int green, unsigned short int blue) {
	char* colorHexCode = malloc(8*sizeof(char));
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
unsigned int getIntCode(struct pixel p, unsigned short int red,
						unsigned short int green, unsigned short int blue) {
	// $intCode = red\times 256^2 + green \times 256 + blue$
	return p.pixelBytes[red]*65536 + p.pixelBytes[green]*256
			+ p.pixelBytes[blue];
}
// >>>
// Is Allocated <<<
/*! @brief Return 1 if the color is already allocated, 0 else
 *
 * @param intCode the int code of the color to be checked
 * @return 1 if the color is already allocated, 0 else
 */
byte isAllocated(unsigned int intCode) {
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
void affiche(Display *dpy, Window w, GC gc, struct pixel* tab,
			unsigned int width, unsigned int height, unsigned short int red,
			unsigned short int green, unsigned short int blue) {
	unsigned int i , j ;

	for(i = 0 ; i < width ; i++) {
		for(j = 0 ; j < height ; j++) {
			struct pixel* p = &tab[i + (j*width)];

			char* colorHexCode = getHexCode(*p, red, green, blue);
			int   colorIntCode = getIntCode(*p, red, green, blue);
			XColor color;
			Colormap cmap = DefaultColormap(dpy, 0);
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
void initRandomly(struct pixel* tab,unsigned int tabWidth,
					unsigned int tabHeight) {
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
void initFromBMP(struct pixel* tab, FILE* f,
					unsigned int pixelStart, unsigned int pixelEnd,
					unsigned short int bpp, unsigned short int paddingSize,
					unsigned int tabWidth, unsigned int tabHeight) {

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
// Color Position <<<
/*! @brief Return the position of a color considering its bitmask
 *
 * @param colorMask the bitmask of the color
 * @return The position of the color
 */
unsigned short int colorPosition(unsigned int colorMask) {
	// $Position = \log_{256}\left(\dfrac{mask}{255}\right)$
	return log(colorMask/255.)/log(256);
}
// >>>

int main (int argc, char const* argv[]) {

	FILE* f;
	unsigned short int bpp, red = 2, green = 1, blue = 0;
	unsigned int pixelStart, pixelEnd;

	for(int i = 0 ; i < COLOR_COUNT ; ++i)
		allocatedColors[i].allocated = 0;

	srand(time(NULL));

	// ====== Check command-line usage ====== //
	if(argc != 2) {
		fprintf(stderr, "Usage: %s {--random | file.bmp}\n", argv[0]);
		return EX_USAGE;
	}

	unsigned int width, height;

	if(strcmp(argv[1], "--random")) {
		f = fopen(argv[1], "rb");

		// ====== Check if readable file ====== <<<
		if(f == NULL) {
			fprintf(stderr, "Error: Could not load the file \"%s\"\n", argv[1]);
			return EX_NOINPUT;
		}
		// >>>
		// ====== BMP file magic check ====== <<<
		// If it really is a BMP, the first two bytes are the ASCII code of "BM"
		char magic[3];
		// Read the first two bytes one time starting from the 'magi'c memory block
		fread(&magic, 2, 1, f);
		if (!(magic[0] == 'B' && magic[1] == 'M')) {
			fprintf(stderr, "Error: Not a BMP file\n");
			return EX_DATAERR;
		}
		// >>>
		// ====== Width and Height ====== <<<
		// width and height located at offsets 0x12 and 0x12
		fseek(f, 0x12, SEEK_SET);
		// Load the width and height into the corresponding variables (4 bytes each)
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
		printf("Pixel array starts at offset: %X and ends at offset: %X\n",
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
		}
	} else {
		width = DEFAULT_WIDTH, height = DEFAULT_WIDTH;
	}
	// >>>

	struct pixel pic[width*height];

	if(strcmp(argv[1], "--random")) {
		// ====== Padding Size ====== <<<

		// There is a padding column until the row reaches a multiple of 4 bytes
		// $bytesPerRow = rowSize \times bytesPerPixel$
		// $paddingSize = \begin{dcases*}0 & if $bytesPerRow \% 4 = 0$\\4 - (bytesPerRow \% 4) & else\end{dcases*}$
		unsigned short int paddingSize = (4 - ((width * bpp) % 4) ) % 4;
		// >>>
		// Read pixel array
		initFromBMP(pic, f, pixelStart, pixelEnd, bpp, paddingSize, width, height);
		fclose(f);
	} else {
		initRandomly(pic, width, height);
	}

	// ====== X11 initialization ====== <<<
	XEvent e;
	Display *dpy = XOpenDisplay(NULL); //pointeur sur un ecran
	int noir = BlackPixel(dpy, DefaultScreen(dpy));
	// creation fenetre : taille, couleur... :
	Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
			width, height, 0, noir, noir);
	XMapWindow(dpy, w); // Affiche la fenetre sur l'ecran
	GC gc = XCreateGC(dpy,w,0,NULL); //On a besoin d'un Graphic Context pour dessiner
	// Il faut attendre l'autorisation de dessiner
	XSelectInput(dpy, w, StructureNotifyMask);

	while (e.type != MapNotify)
		XNextEvent(dpy, &e);
	// >>>

	affiche(dpy, w, gc, pic, width, height, red, green, blue);
	sleep(5);

	return 0;
}
// vim: fdm=marker:fmr=<<<,>>>
