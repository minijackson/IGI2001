#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <math.h>

#ifndef DEFAULT_HEIGHT
#define DEFAULT_HEIGHT 200
#endif

#ifndef DEFAULT_WIDTH
#define DEFAULT_WIDTH 200
#endif

#define ALPHA_FIRST 0
#define ALPHA_LAST  1

typedef unsigned char byte;

struct pixel {
	byte pixelBytes[4];
};

void affiche(Display *dpy, Window w, GC gc, struct pixel* tab,
			unsigned int width, unsigned int height, unsigned short int red,
			unsigned short int green, unsigned short int blue) {
	unsigned int i , j ;

	for(i = 0 ; i < width ; i++) {
		for(j = 0 ; j < height ; j++) {
			struct pixel* p = &tab[i + (j*width)];

			Colormap cmap = DefaultColormap(dpy, 0);
			char colorHexCode[8];
			sprintf(colorHexCode, "#%X%X%X",
					p->pixelBytes[red], p->pixelBytes[green], p->pixelBytes[blue]);

			XColor color;
			XParseColor(dpy, cmap, colorHexCode, &color);
			XAllocColor(dpy, cmap, &color);

			XSetForeground(dpy, gc, color.pixel);
			XDrawPoint(dpy, w, gc, i, j);
		}
	}

	XFlush(dpy);
}

void blackout(unsigned char* tab, unsigned int width, unsigned int height) {
	unsigned int i;
	for(i = 0; i < width*height; i++)
		tab[i] = 0;
}

void initFromBMP(struct pixel* tab, FILE* f,
					unsigned int pixelStart, unsigned int pixelEnd,
					unsigned short int bpp, unsigned short int paddingSize,
					unsigned int tabWidth, unsigned int tabHeight,
					unsigned short int red, unsigned short int green,
					unsigned short int blue) {

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

unsigned short int colorPosition(unsigned int colorMask) {
	return log2(pow(colorMask/255., 1/8.));
}

int main (int argc, char const* argv[]) {
	// ====== Check command-line usage ====== //
	if(argc != 2) {
		fprintf(stderr, "Usage: %s file.bmp\n", argv[0]);
		return EX_USAGE;
	}

	FILE* f = fopen(argv[1], "rb");

	// ====== Check if readable file ====== //
	if(f == NULL) {
		fprintf(stderr, "Error: Could not load the file \"%s\"\n", argv[1]);
		return EX_NOINPUT;
	}

	// ====== BMP file magic check ====== //
	// If it really is a BMP, the first two bytes are the ASCII code of "BM"
	char magic[3];
	// Read the first two bytes one time starting from the 'magi'c memory block
	fread(&magic, 2, 1, f);
	if (!(magic[0] == 'B' && magic[1] == 'M')) {
		fprintf(stderr, "Error: Not a BMP file\n");
		return EX_DATAERR;
	}

	// ====== Pixel array offset ====== //
	unsigned int pixelStart, pixelEnd;
	fseek(f, 0xA, SEEK_SET);
	fread(&pixelStart, 4, 1, f);
	// Go to the end
	fseek(f, 0, SEEK_END);
	pixelEnd = ftell(f);
	printf("Pixel array starts at offset: %X and ends at offset: %X\n",
			pixelStart, pixelEnd);

	// ====== Width and Height ====== //
	unsigned int width, height;
	// width and height located at offsets 0x12 and 0x12
	fseek(f, 0x12, SEEK_SET);
	// Load the width and height into the corresponding variables (4 bytes each)
	fread(&width, 4, 1, f);
	fread(&height, 4, 1, f);

	printf("Height = %u, Width = %u\n", height, width);

	// ====== Bytes per pixels ====== //
	unsigned short int bpp;
	// The number of bytes per pixels located at offset 0x1C
	fseek(f, 0x1C, SEEK_SET);
	fread(&bpp, 2, 1, f);
	// Convert to bytes
	bpp /= 8;

	unsigned short int red = 2, green = 1, blue = 0, alpha = 42;
	printf("Number of bytes per pixels: %hu\n", bpp);
	if(bpp == 4) {
		fprintf(stderr, "Warning: Alpha channel will not be displayed.\n");
		unsigned int redMask, greenMask, blueMask, alphaMask;
		fseek(f, 0x36, SEEK_SET);
		fread(&redMask, 4, 1, f);
		red = colorPosition(redMask);
		fread(&greenMask, 4, 1, f);
		green = colorPosition(greenMask);
		fread(&blueMask, 4, 1, f);
		blue = colorPosition(blueMask);
		fread(&alphaMask, 4, 1, f);
		alpha = colorPosition(alphaMask);
	} else if(bpp != 3) {
		fprintf(stderr, "Error: Only RGB and RGBA file supported.\n");
	}

	// ====== Padding Size ====== //

	// There is a padding column until the row reaches a multiple of 4 bytes
	// $bytesPerRow = rowSize \times bytesPerPixel$
	// $paddingSize = \begin{dcases*}0 & if $bytesPerRow \% 4 = 0$\\4 - (bytesPerRow \% 4) & else\end{dcases*}$
	unsigned short int paddingSize = (4 - ((width * bpp) % 4) ) % 4;

	struct pixel pic[width*height];

	// ====== Read pixel array ====== //
	initFromBMP(pic, f, pixelStart, pixelEnd, bpp, paddingSize, width, height,
				red, green, blue);
	fclose(f);

	/* // Picture initialization */
	/* blackout(pic, width, height); */

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

	affiche(dpy, w, gc, pic, width, height, red, green, blue);
	sleep(10); //on attend 10s avant de quitter

	return 0;
}
