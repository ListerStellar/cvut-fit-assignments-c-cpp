#include <stdio.h>
#include <math.h>

int nod(int x, int y) {
	int tmpx;
	while (y != 0) {
		tmpx = x;
		x = y;
		y = tmpx % y;
	}
	return x;
}

int get_triangles_num(int lo, int hi) {
	if (lo == hi) return 0;
	int trg_num = 0;
	int k, m = 2;
	int side1, side2, side3, kside1, kside2, kside3;
	
	while (m * m <= hi) {
		for (int i = 1; i < m; i++) {
			if ((m - i) % 2 == 1 && nod(m, i) == 1) {
				side1 = m * m - i * i;
				side2 = 2 * m * i;
				side3 = m * m + i * i;
				k = 1;
				while (k * side3 <= hi) {
					kside1 = k * side1;
					kside2 = k * side2;
					kside3 = k * side3;
					if (kside1 >= lo && kside1 <= hi && kside2 >= lo && kside2 <= hi && kside3 >= lo && kside3 <= hi) trg_num++;
					k++;
				}
			}
		}
		m++;
	}

	return trg_num;
}

int print_triangles(int lo, int hi) {
	//int trg_num = get_triangles_num(lo, hi);
	if (lo == hi) {
		printf("Celkem: %d\n", 0);
		return 0;
	}
	
	int trg_num = 0;
	for (int i = hi; i > lo+1; i--) {
		for (int j = i - 1; j >= lo; j--) {
			for (int c = j - 1; c >= lo; c--) {
				if (c * c + j * j == i * i) {
					printf("* %d %d %d\n", c, j, i);
					trg_num++;
				}
			}
		}
	}

	printf("Celkem: %d\n", trg_num);
	return 0;
}

int mn_func(char typ, int lo, int hi) {
	if (typ == '?') {
		print_triangles(lo, hi);
		return 0;
	}
	if (typ == '#') {
		printf("Celkem: %d\n", get_triangles_num(lo,hi));
		return 0;
	}

	printf("Nespravny vstup.\n");
	return 1;
}

int main()
{
	char typ, zvr1, zvr2, strd;
	int lo, hi, errcode = 0;
	printf("Problemy:\n");

	while (errcode != -1) {
		errcode = scanf(" %c", &typ);
		if (errcode == -1) return 0;
		if (errcode != 1 || (typ != '?' && typ != '#')) {
			printf("Nespravny vstup.\n");
			return 1;
		}
		if (scanf(" %c", &zvr1) != 1 || zvr1 != '<') {
			printf("Nespravny vstup.\n");
			return 2;
		}
		if (scanf("%d", &lo) != 1 || lo <= 0) {
			printf("Nespravny vstup.\n");
			return 3;
		}
		if (scanf(" %c", &strd) != 1 || strd != ';') {
			printf("Nespravny vstup.\n");
			return 4;
		}
		if (scanf("%d", &hi) != 1 || hi <= 0 || lo > hi) {
			printf("Nespravny vstup.\n");
			return 5;
		}
		if (scanf(" %c", &zvr2) != 1 || zvr2 != '>') {
			printf("Nespravny vstup.\n");
			return 6;
		}
		mn_func(typ, lo, hi);
	}
	
	return 0;
}
