#include <stdio.h>
#include <math.h>
#include <float.h>

bool dcomp(double x, double y) {
	double rzn = fabs(x - y);
	x = fabs(x); y = fabs(y);
	double newEps = DBL_EPSILON * fmax(x, y) * 1000;
	//printf("\ndiff=%le\nscaledEpsilon=%le\n\n",rzn,newEps);
	//printf("diff <= scaledEpsilon -> %b\n\n", rzn <= newEps);
	return rzn < newEps;
	//return rzn <= newEps;
}

bool ddots3_inline(double ax, double ay, double bx, double by, double cx, double cy) {
	//printf("a=%le\nb=%le\n", (ax - bx) / (cx - bx), (ay - by) / (cy - by));
	return dcomp((ax - bx) / (cx - bx), (ay - by) / (cy - by));
}

double dsqrlen(double ax, double ay, double bx, double by) {
	return (bx - ax) * (bx - ax) + (by - ay) * (by - ay);
}

double dmid(double a, double b) {
	return (a + b) / 2;
}

const char* dtypeofrec(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy) {
	double ab, bc, cd, ad, ac, bd;
	ab = dsqrlen(ax, ay, bx, by); bc = dsqrlen(bx, by, cx, cy); cd = dsqrlen(cx, cy, dx, dy); ad = dsqrlen(ax, ay, dx, dy);
	ac = dsqrlen(ax, ay, cx, cy); bd = dsqrlen(bx, by, dx, dy);
	if (dcomp(ab, bc) && dcomp(bc, cd) && dcomp(cd, ad) && dcomp(ad, ab)) {
		if (dcomp(ac, bd)) {
			return "ctverec";
		}
		else {
			return "kosoctverec";
		}
	}
	else if (dcomp(ac,bd)) {
		return "obdelnik";
	}
	return "rovnobeznik";
}

int main()
{
	printf("Bod A:\n");
	char zvr1, crk, zvr2;
	double ax, ay;
	int errcode = scanf(" %c %le %c %le %c", &zvr1, &ax, &crk, &ay, &zvr2);

	if (errcode != 5 || zvr1 != '[' || crk != ',' || zvr2 != ']') {
		printf("Nespravny vstup.\n");
		return 1;
	}

	printf("Bod B:\n");
	char zvr1_1, crk_1, zvr2_1;
	double bx, by;
	int errcode1 = scanf(" %c %le %c %le %c", &zvr1_1, &bx, &crk_1, &by, &zvr2_1);

	if (errcode1 != 5 || zvr1_1 != '[' || crk_1 != ',' || zvr2_1 != ']') {
		printf("Nespravny vstup.\n");
		return 2;
	}

	printf("Bod C:\n");
	char zvr1_2, crk_2, zvr2_2, nd;
	double cx, cy;
	int errcode2 = scanf(" %c %le %c %le %c %c", &zvr1_2, &cx, &crk_2, &cy, &zvr2_2, &nd);

	if (errcode2 != 5 || zvr1_2 != '[' || crk_2 != ',' || zvr2_2 != ']') {
		printf("Nespravny vstup.\n");
		return 2;
	}

	if (ddots3_inline(ax, ay, bx, by, cx, cy) || (dcomp(ax,bx) && dcomp(ay,by)) || (dcomp(ax, cx) && dcomp(ay, cy)) || (dcomp(bx, cx) && dcomp(by, cy))) {
		printf("Rovnobezniky nelze sestrojit.\n");
		return 0;
	}

	double aax, aay, midx, midy;
	midx = dmid(bx, cx); midy = dmid(by, cy);
	aax = (midx - ax) * 2 + ax; aay = (midy - ay) * 2 + ay;

	double bbx, bby;
	midx = dmid(ax, cx); midy = dmid(ay, cy);
	bbx = (midx - bx) * 2 + bx; bby = (midy - by) * 2 + by;

	double ccx, ccy;
	midx = dmid(bx, ax); midy = dmid(by, ay);
	ccx = (midx - cx) * 2 + cx; ccy = (midy - cy) * 2 + cy;




	printf("A': [%.20g,%.20g], %s\nB': [%.20g,%.20g], %s\nC': [%.20g,%.20g], %s\n", aax, aay, dtypeofrec(ax,ay,bx,by,aax,aay,cx,cy), bbx, bby, dtypeofrec(ax,ay,bx,by,cx,cy,bbx,bby), ccx, ccy, dtypeofrec(ax,ay,ccx,ccy,bx,by,cx,cy));

	return 0;
}
