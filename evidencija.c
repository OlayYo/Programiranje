#define _CRT_SECURE_NO_WARNINGS
#include "header.h"

// -> Funkcija za prikaz statusa kao string
const char* statusToString(StatusPopravka status) {
    switch (status) {
    case STATUS_CEKA_DIJELOVE: return "ČEKA DIJELOVE";
    case STATUS_POPRAVLJENO:   return "POPRAVLJENO";
    case STATUS_U_TOKU:        return "U TOKU";
    case STATUS_ODBIJENO:      return "ODBIJENO";
    default:                   return "NEPOZNATO";
    }
}

// -> 1. Create (dodavanje)
void dodajKvarPopravak(KvarPopravak** glava) {
    KvarPopravak* novi = (KvarPopravak*)malloc(sizeof(KvarPopravak)); // -> 16. malloc
    if (!novi) {
        perror("Greska pri alokaciji memorije");
        return;
    }
    printf("Unesi registraciju vozila: ");
    scanf("%11s", novi->registracija);
    printf("Unesi opis kvara: ");
    getchar(); // flush
    fgets(novi->opis, sizeof(novi->opis), stdin);
    novi->opis[strcspn(novi->opis, "\n")] = 0;
    printf("Unesi datum (dd.mm.gggg): ");
    scanf("%10s", novi->datum);
    printf("Unesi cijenu popravka: ");
    scanf("%f", &novi->cijena);
    printf("Status (0-CEKA DIJELOVE, 1-POPRAVLJENO, 2-U TOKU, 3-ODBIJENO): ");
    int status;
    scanf("%d", &status);
    novi->status = (StatusPopravka)status;
    novi->next = *glava;
    *glava = novi;
    printf("Kvar je dodan.\n");
}

// -> 1. Read (ispis)
void ispisiSveKvarove(const KvarPopravak* glava) {
    const KvarPopravak* kvar = glava;
    int rb = 1;
    while (kvar) {
        printf("Redni broj: %d\nRegistracija: %s\nOpis: %s\nDatum: %s\nCijena: %.2f\nStatus: %s\n----------------\n",
            rb++, kvar->registracija, kvar->opis, kvar->datum, kvar->cijena, statusToString(kvar->status));
        kvar = kvar->next;
    }
}

// -> 1. Update (ažuriranje)
void azurirajKvarPopravak(KvarPopravak* glava) {
    char reg[12] = { 0 };
    printf("Unesi registraciju vozila za ažuriranje: ");
    scanf("%11s", reg);
    for (KvarPopravak* kvar = glava; kvar; kvar = kvar->next) {
        if (strcmp(kvar->registracija, reg) == 0) {
            printf("Unesi novi opis: ");
            getchar();
            fgets(kvar->opis, sizeof(kvar->opis), stdin);
            kvar->opis[strcspn(kvar->opis, "\n")] = 0;
            printf("Unesi novu cijenu: ");
            scanf("%f", &kvar->cijena);
            printf("Unesi novi status (0-CEKA DIJELOVE, 1-POPRAVLJENO, 2-U TOKU, 3-ODBIJENO): ");
            int status;
            scanf("%d", &status);
            kvar->status = (StatusPopravka)status;
            printf("Kvar ažuriran.\n");
            return;
        }
    }
    printf("Kvar nije pronađen.\n");
}

// -> 1. Delete (brisanje)
void obrisiKvarPopravak(KvarPopravak** glava) {
    char reg[12] = { 0 };
    printf("Unesi registraciju vozila za brisanje: ");
    scanf("%11s", reg);
    KvarPopravak* prev = NULL;
    KvarPopravak* curr = *glava;
    while (curr) {
        if (strcmp(curr->registracija, reg) == 0) {
            if (prev) prev->next = curr->next;
            else *glava = curr->next;
            free(curr); // -> 18. free
            printf("Kvar obrisan.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Kvar nije pronađen.\n");
}

// -> 23. Sortiranje (po datumu, bubble sort za listu)
void sortirajKvarove(KvarPopravak** glava) {
    if (!*glava || !(*glava)->next) return;
    int zamjena;
    do {
        zamjena = 0;
        KvarPopravak* prev = NULL;
        KvarPopravak* curr = *glava;
        while (curr->next) {
            if (strcmp(curr->datum, curr->next->datum) > 0) {
                KvarPopravak* tmp = curr->next;
                curr->next = tmp->next;
                tmp->next = curr;
                if (prev) prev->next = tmp;
                else *glava = tmp;
                prev = tmp;
                zamjena = 1;
            }
            else {
                prev = curr;
                curr = curr->next;
            }
        }
    } while (zamjena);
    printf("Kvarovi sortirani po datumu.\n");
}

// -> 24. Pretraživanje (po registraciji)
void pretraziKvarPopravak(const KvarPopravak* glava) {
    char reg[12] = { 0 };
    printf("Unesi registraciju za pretragu: ");
    scanf("%11s", reg);
    for (const KvarPopravak* kvar = glava; kvar; kvar = kvar->next) {
        if (strcmp(kvar->registracija, reg) == 0) {
            printf("Kvar pronađen: %s, opis: %s\n", kvar->registracija, kvar->opis);
            return;
        }
    }
    printf("Nema kvara za tu registraciju.\n");
}

// -> 19. Datoteke (binarno spremanje/učitavanje)
void spremiKvarove(const KvarPopravak* glava, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke za spremanje");
        return;
    }
    int broj = 0;
    for (const KvarPopravak* kvar = glava; kvar; kvar = kvar->next) broj++;
    fwrite(&broj, sizeof(int), 1, fp);
    for (const KvarPopravak* kvar = glava; kvar; kvar = kvar->next) {
        fwrite(kvar, sizeof(KvarPopravak) - sizeof(KvarPopravak*), 1, fp); // -> 15. statička polja
    }
    fclose(fp);
    printf("Podaci spremljeni u datoteku.\n");
}

void ucitajKvarove(KvarPopravak** glava, const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke za ucitavanje");
        return;
    }
    oslobodiMemoriju(glava); // -> 18. Oslobađanje stare memorije
    int broj = 0;
    fread(&broj, sizeof(int), 1, fp);
    for (int i = 0; i < broj; ++i) {
        KvarPopravak* novi = (KvarPopravak*)malloc(sizeof(KvarPopravak));
        if (!novi) {
            perror("Greska pri alokaciji memorije");
            fclose(fp);
            return;
        }
        fread(novi, sizeof(KvarPopravak) - sizeof(KvarPopravak*), 1, fp);
        novi->next = *glava;
        *glava = novi;
    }
    fclose(fp);
    printf("Podaci učitani iz datoteke.\n");
}

// -> 18. Oslobađanje memorije
void oslobodiMemoriju(KvarPopravak** glava) {
    while (*glava) {
        KvarPopravak* tmp = *glava;
        *glava = (*glava)->next;
        free(tmp);
    }
}
