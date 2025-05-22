#define _CRT_SECURE_NO_WARNINGS
#include "header.h"

// -> 11. enum za izbornik
typedef enum {
    DODAJ = 1,
    ISPIS,
    AZURIRAJ,
    OBRISI,
    SORTIRAJ,
    PRETRAZI,
    SPREMI,
    UCITAJ,
    IZLAZ
} IzbornikOpcije;

int main(void) {
    KvarPopravak* glava = NULL; // -> 12. Pokazivač na početak liste
    int odabir = 0;
    const char* filename = "kvarovi.bin";
    do {
        printf("\n--- Evidencija kvarova i popravaka vozila ---\n");
        printf("1 -> Dodaj novi kvar/popravak\n");
        printf("2 -> Ispisi sve kvarove\n");
        printf("3 -> Azuriraj kvar\n");
        printf("4 -> Obrisi kvar\n");
        printf("5 -> Sortiraj po datumu\n");
        printf("6 -> Pretrazi po registraciji\n");
        printf("7 -> Spremi u datoteku\n");
        printf("8 -> Ucitaj iz datoteke\n");
        printf("9 -> Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &odabir);

        switch (odabir) {
        case DODAJ:
            dodajKvarPopravak(&glava);
            break;
        case ISPIS:
            ispisiSveKvarove(glava);
            break;
        case AZURIRAJ:
            azurirajKvarPopravak(glava);
            break;
        case OBRISI:
            obrisiKvarPopravak(&glava);
            break;
        case SORTIRAJ:
            sortirajKvarove(&glava);
            break;
        case PRETRAZI:
            pretraziKvarPopravak(glava);
            break;
        case SPREMI:
            spremiKvarove(glava, filename);
            break;
        case UCITAJ:
            ucitajKvarove(&glava, filename);
            break;
        case IZLAZ:
            oslobodiMemoriju(&glava);
            printf("Izlazak iz programa.\n");
            break;
        default:
            printf("Pogresan unos!\n");
        }
    } while (odabir != IZLAZ);

    return 0;
}
