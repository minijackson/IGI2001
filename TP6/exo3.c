#include <stdio.h>
#include <stdlib.h>

struct np_cmpx {
	float re;
	float im;
};

void affiche_complexe(const struct np_cmpx nb) {
	printf("%f + %fi\n", nb.re, nb.im);
}

struct np_cmpx add_cmpx(const struct np_cmpx x, const struct np_cmpx y) {
	// $ a + b = \mathfrak{R}(a) + \mathfrak{R}(b) + \left(\mathfrak{I}(a) + \mathfrak{I}(b)\right) i$
	struct np_cmpx ret = {x.re + y.re, x.im + y.im};
	return ret;
}

struct np_cmpx mul_cmpx(const struct np_cmpx x, const struct np_cmpx y) {
	// $\left(a_0+b_0 i\right)\times\left(a_1+b_1 i\right) = a_0\times a_1 - b_0 \times b_1 + \left(b_0\times a_1 + a_0\times b_1\right) i$
	struct np_cmpx ret = {(x.re * y.re) - (x.im * y.im),
						  (x.im * y.re) + (x.re * y.im)};
	return ret;
}

void permute_complexe(struct np_cmpx* nb) {
	// Store the real value in temporary variable
	int temp = nb->re;
	nb->re = nb->im;
	nb->im = temp;
}

int main (int argc, char const* argv[]) {
	if (argc != 5) {
		printf("Usage: %s a b c d\n", argv[0]);
	} else {
		struct np_cmpx x = {atof(argv[1]), atof(argv[2])},
					   y = {atof(argv[3]), atof(argv[4])},
					   z;

		affiche_complexe(x);
		affiche_complexe(y);
		affiche_complexe(z);

		affiche_complexe(add_cmpx(x, y));
		affiche_complexe(mul_cmpx(x, y));

		permute_complexe(&x);
		affiche_complexe(x);

	}
	return 0;
}
