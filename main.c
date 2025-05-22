#define _CRT_SECURE_NO_WARNINGS

#include "header.h"

// 11. enum za izbornik, 5. Imenovanje identifikatora
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
    KvarPopravak* glava = NULL; // 12. Pokazivač na početak liste
    int odabir = 0;             // 2. int
    const char* filename = "kvarovi.bin"; // 2. char*, 15. Statički zauzeto polje
    do {
        // 10. Izbornik
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

        switch (odabir) { // 10. Izbornik, 11. enum
        case DODAJ:
            dodajKvarPopravak(&glava);   // 1. Create
            break;
        case ISPIS:
            ispisiSveKvarove(glava);     // 1. Read
            break;
        case AZURIRAJ:
            azurirajKvarPopravak(glava); // 1. Update
            break;
        case OBRISI:
            obrisiKvarPopravak(&glava);  // 1. Delete
            break;
        case SORTIRAJ:
            sortirajKvarove(&glava);     // 23. Sortiranje
            break;
        case PRETRAZI:
            pretraziKvarPopravak(glava); // 24. Pretraživanje
            break;
        case SPREMI:
            spremiKvarove(glava, filename); // 19. Datoteke
            break;
        case UCITAJ:
            ucitajKvarove(&glava, filename); // 19. Datoteke
            break;
        case IZLAZ:
            oslobodiMemoriju(&glava);    // 18. Oslobađanje memorije
            printf("Izlazak iz programa.\n");
            break;
        default:
            printf("Pogresan unos!\n");
        }
    } while (odabir != IZLAZ);

    return 0;
}
