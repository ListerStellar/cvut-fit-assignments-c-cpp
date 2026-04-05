#include <stdio.h>
#include <math.h>
#include <float.h>

bool dcomp(double x, double y) {
	double rzn = fabs(x - y);
	x = fabs(x); y = fabs(y);
	double newEps = DBL_EPSILON * fmax(x, y) * 1000;
	return rzn < newEps;
}

int main()
{
	printf("Obdelnik #1:\n");
	double a, b;
	int errcode = scanf(" %le %le", &a, &b);
	if (errcode != 2 || a <= 0 || b <= 0) {
		printf("Nespravny vstup.\n");
		return 1;
	}
	printf("Obdelnik #2:\n");
	double a1, b1;
	char nd;
	int errcode1 = scanf(" %le %le %c", &a1, &b1, &nd);
	if (errcode1 != 2 || a1 <= 0 || b1 <= 0) {
		printf("Nespravny vstup.\n");
		return 2;
	}

	double sqr1, sqr2;
	sqr1 = a * b; sqr2 = a1 * b1;
	if (dcomp(sqr1, sqr2)) {
		printf("Obdelniky maji stejny obsah.\n");
		return 0;
	}
	if (sqr1 > sqr2) {
		printf("Obdelnik #2 ma mensi obsah.\n");
		return 0;
	}
	if (sqr1 < sqr2) {
		printf("Obdelnik #1 ma mensi obsah.\n");
		return 0;
	}
	return 0;
}
