#include <X11/Xlib.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#ifndef HEIGHT
#define HEIGHT 200
#endif

#ifndef WIDTH
#define WIDTH 200
#endif

void dessine(Display* dpy, Window w, GC gc, int col) {
	XSetForeground(dpy, gc, col); //Couleur du stylo
	XDrawLine(dpy, w, gc, 0, 0, WIDTH, HEIGHT);
	XDrawPoint(dpy, w, gc, 10, 50);
	XDrawPoint(dpy, w, gc, 10, 51);
	XFlush(dpy); //Force l'affichage
}

void affiche(Display *dpy, Window w, GC gc, unsigned char * T) {
	int i , j ;
	int blanc = WhitePixel(dpy, DefaultScreen(dpy));
	int noir = BlackPixel(dpy, DefaultScreen(dpy));

	for(i = 0 ; i < WIDTH ; i++) {
		for(j = 0 ; j < HEIGHT ; j++) {
			XSetForeground(dpy, gc, (T[i + (j*HEIGHT)] == 1) ? blanc : noir);
			XDrawPoint(dpy, w, gc, i, j);
		}
	}

	XFlush(dpy);
}

void blackout(unsigned char* tab) {
	unsigned int i;
	for(i = 0; i < HEIGHT*WIDTH; i++)
		tab[i] = 0;
}

void creeCarre(unsigned int x0, unsigned int y0,
			   unsigned int x1, unsigned int y1, unsigned char* tab) {
	unsigned int i, j;
	// Check if square is inside boundaries
	if((x0 <= WIDTH) && (x1 <= WIDTH) && (y0 <= HEIGHT) && (y1 <= HEIGHT)) {
		for(i = x0 ; i <= x1 ; i++)
			tab[(y0*WIDTH)+i] = tab[(y1*WIDTH)+i] = 1;
		for(j = y0 ; j <= y1 ; j++)
			tab[(j*WIDTH)+x0] = tab[(j*WIDTH)+x1] = 1;
	}
}

void creeCercle(unsigned int x0, unsigned int y0,
				unsigned int r, unsigned char* tab) {
	int i;
	for(i = (int)x0-(int)r; i < (int)(x0+r); i++) {
		// If i is outside boundaries, no need to draw
		if(i < HEIGHT) {
			// $\left(x-x_0\right)^2 + \left(y-y_0\right)^2 = r^2 \Leftrightarrow y = \pm \sqrt{r^2 - \left(x-x_0\right)^2} + y_0$
			/* abs is necessary as pow fails with negative values */
			float racine = sqrt(powf(r, 2) - powf(abs(i-x0), 2));
			float yp = y0 + racine;
			float yn = y0 - racine;
			// Upper part and lower part of the circle

			// Check boundaries
			if(yp > 0 && WIDTH > yp)
				tab[(int)(i*HEIGHT) + (int)yp] = 1;
			if(yn > 0 && WIDTH > yn)
				tab[(int)(i*HEIGHT) + (int)yn] = 1;
		}
	}
}

void creeCercleTrigo(unsigned int x0, unsigned int y0,
					 unsigned int r, unsigned char* tab) {
	float i;
	// $ \Delta\big(M(t), M(t+s)\big) \leq \sqrt{2} \Leftrightarrow s \leq \cos^{-1}\left(1-\dfrac{1}{r^2}\right) \simeq \dfrac{1.41}{r}$
	float step = 1.41 / r;
	// $t\in\left[0,2\pi\right]$
	for(i = 0; i < 6.3; i += step) {
		// $\begin{pmatrix}x\\y\end{pmatrix} = \begin{pmatrix}r\times\cos(t)+x_0\\r\times\sin(t)+y_0\end{pmatrix}$
		int x = (int)(r*cos(i) + x0);
		int y = (int)(r*sin(i) + y0);

		if(x > 0 && WIDTH > x && y > 0 && HEIGHT > y)
			tab[(int)(y*HEIGHT) + (int)x] = 1;
	}
}

int main () {
	unsigned char pic[HEIGHT*WIDTH];
	// Picture initialization
	blackout(pic);

	XEvent e;
	Display *dpy = XOpenDisplay(NULL); //pointeur sur un ecran
	int noir = BlackPixel(dpy, DefaultScreen(dpy));
	int blanc = WhitePixel(dpy, DefaultScreen(dpy));
	// creation fenetre : taille, couleur... :
	Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
			WIDTH, HEIGHT, 0, noir, noir);
	XMapWindow(dpy, w); // Affiche la fenetre sur l'ecran
	GC gc = XCreateGC(dpy,w,0,NULL); //On a besoin d'un Graphic Context pour dessiner
	// Il faut attendre l'autorisation de dessiner
	XSelectInput(dpy, w, StructureNotifyMask);

	while (e.type != MapNotify)
		XNextEvent(dpy, &e);
	// On dessine:
	dessine(dpy, w, gc, blanc);

	creeCarre(10, 10, 42, 69, pic);
	creeCercle(100, 100, 100, pic);

	int i;
	// Concentric circles
	for (i = 10; i <= HEIGHT; i += 30) {
		creeCercle(150, 150, i, pic);
		creeCercleTrigo(50, 150, i, pic);
	}

	affiche(dpy, w, gc, pic);
	sleep(10); //on attend 10s avant de quitter
	return 0;
}
