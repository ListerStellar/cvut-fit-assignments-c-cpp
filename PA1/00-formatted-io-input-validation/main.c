#include <stdio.h>

int main()
{

    printf("ml' nob:\n");
    char add_c;
    int num;
    int errcode = scanf(" %d %c", &num, &add_c);

    if (errcode == 0 || errcode == -1) {
        printf("Neh mi'\n");
        return 1;
    }
    if (errcode != 1) {
        printf("bIjatlh 'e' yImev\n");
        return 2;
    }
    if (0 > num || 8 < num) {
        printf("Qih mi' %d\n", num);
        return 3;
    }

    if (num == 0) {
        printf("Qapla'\nnoH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n");
        return 0;
    }
    if (num == 1) {
        printf("Qapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n");
        return 0;
    }
    if (num == 2) {
        printf("Qapla'\nQu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n");
        return 0;
    }
    if (num == 3) {
        printf("Qapla'\nbISeH'eghlaH'be'chugh latlh Dara'laH'be'.\n");
        return 0;
    }
    if (num == 4) {
        printf("Qapla'\nqaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n");
        return 0;
    }
    if (num == 5) {
        printf("Qapla'\nSuvlu'taHvIS yapbe' HoS neH.\n");
        return 0;
    }
    if (num == 6) {
        printf("Qapla'\nHa'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n");
        return 0;
    }
    if (num == 7) {
        printf("Qapla'\nHeghlu'meH QaQ jajvam.\n");
        return 0;
    }
    if (num == 8) {
        printf("Qapla'\nleghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
        return 0;
    }

    return 4;
}
