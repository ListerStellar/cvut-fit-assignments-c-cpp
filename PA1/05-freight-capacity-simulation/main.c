#include <stdio.h>
#include <stdlib.h>

typedef struct {
	long long from, to, capacity, pricePerDay;
} t_dprost;

int compare_from(const void* a, const void* b) {
	t_dprost val1 = *(const t_dprost*)a;
	t_dprost val2 = *(const t_dprost*)b;
	return (val1.from > val2.from) - (val1.from < val2.from);
}

int inputDopravniProstredky(t_dprost* dprostredky, size_t* dp_size) {
	char zv_s, zv1, zv2, pml, cm1, cm2;
	long long from, to, capacity, pricePerDay;
	int dpros_num = 0;
	*dp_size = dpros_num;
	printf("Moznosti dopravy:\n");
	if (scanf(" %c", &zv_s) != 1 || zv_s != '{') return 1;
	if (scanf(" %c", &zv1) != 1) return 2;
	//if (zv1 == '}') return 0;
	if (zv1 != '[') return 3;
	if (scanf("%lld %c%lld %c%lld %c%lld %c", &from, &pml, &to, &cm1, &capacity, &cm2, &pricePerDay, &zv2) != 8 || from < 0 || to < 0 || to < from || capacity <= 0 || pricePerDay <= 0 || pml != '-' || cm1 != ',' || cm2 != ',' || zv2 != ']') {
		return 4;
	}
	dpros_num += 1;
	*dp_size = dpros_num;
	dprostredky[dpros_num - 1] = { from, to, capacity, pricePerDay };
	while (true) {
		if (scanf(" %c", &zv1) != 1) return 5;
		if (zv1 == '}') break;
		if (zv1 != ',') return 6;
		if (scanf(" %c%lld %c%lld %c%lld %c%lld %c", &zv1, &from, &pml, &to, &cm1, &capacity, &cm2, &pricePerDay, &zv2) != 9 || from < 0 || to < 0 || to < from || capacity <= 0 || pricePerDay <= 0 || pml != '-' || cm1 != ',' || cm2 != ',' || zv1 != '[' || zv2 != ']') return 7;
		dpros_num += 1;
		*dp_size = dpros_num;
		if (dpros_num > 100000) return 8;
		dprostredky[dpros_num-1] = { from, to, capacity, pricePerDay };
	}
	return 0;
}

int main() {
	// Init

	static t_dprost dprostredky[100000] = {};
	size_t dp_size;
	int errcode = inputDopravniProstredky(dprostredky, &dp_size);
	if (errcode != 0) {
		printf("Nespravny vstup.\n");
		return 1;
	}
	
	// Init

	qsort(dprostredky, dp_size, sizeof(t_dprost), compare_from);
	long long start_day, capa, price;
	printf("Naklad:\n");
	do {
		errcode = scanf("%lld %lld", &start_day, &capa);
		if (errcode == -1) break;
		if (errcode != 2 || start_day < 0 || capa <= 0) {
			printf("Nespravny vstup.\n");
			return 2;
		}

		price = 0;
		while (capa > 0) {
			if (start_day > dprostredky[dp_size - (size_t)1].to) {
				start_day = -1;
				price = -1;
				break;
			}
			for (int i = 0; i < (int)dp_size; i++) {
				if (start_day >= dprostredky[i].from && start_day <= dprostredky[i].to) {
					capa -= dprostredky[i].capacity;
					price += dprostredky[i].pricePerDay;
				}
			}
			start_day += capa > 0 ? 1 : 0;
		}
		if (start_day <= -1 && price <= -1) {
			printf("Prilis velky naklad, nelze odvezt.\n");
		}
		else {
			printf("Konec: %lld, cena: %lld\n", start_day, price);
		}

	} while (errcode != -1);


	return 0;
}