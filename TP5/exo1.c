#include <stdio.h>

float max3float(float f1, float f2, float f3)
{
	float temp;
	return (f1 > ( temp = (f2 > f3) ? f2 : f3 ) ? f1 : temp);
}

int main () {
	float x1 = 3.5, x2 = 10. , x3 = 25.3 , res = 0.;
	printf("%f\n", res = max3float(x1, x2, x3));
	return 0;
}
