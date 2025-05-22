Evidencija kvarova i popravaka vozila

#include "header.h"

// 8. Definicija globalne varijable
int globalBrojKvarova = 0;

// 9. Primjer inline funkcije
inline int zbroji(int a, int b) { return a + b; }

const char* statusToString(StatusPopravka status) {
    switch (status) {
        case STATUS_CEKA_DIJELOVE: return "ČEKA DIJELOVE";
        case STATUS_POPRAVLJENO:   return "POPRAVLJENO";
        case STATUS_U_TOKU:        return "U TOKU";
        case STATUS_ODBIJENO:      return "ODBIJENO";
        default:                   return "NEPOZNATO";
    }
}

void dodajKvarPopravak(KvarPopravak** glava) {
    KvarPopravak* novi = (KvarPopravak*)malloc(sizeof(KvarPopravak));
    if (!novi) {
        perror("Greska pri alokaciji memorije");
        return;
    }
    printf("Unesi registraciju vozila: ");
    scanf("%11s", novi->registracija);
    printf("Unesi opis kvara: ");
    getchar();
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
    globalBrojKvarova++; // 8. Povećavanje globalnog broja kvarova
    printf("Kvar je dodan.\n");
}

void ispisiSveKvarove(const KvarPopravak* glava) {
    static int brojPoziva = 0; // 6. static lokalna varijabla
    brojPoziva++;
    printf("Funkcija ispisiSveKvarove je pozvana %d puta\n", brojPoziva);
    printf("Ukupno upisanih kvarova: %d\n", globalBrojKvarova); // 8. Korištenje globalne varijable
    const KvarPopravak* kvar = glava;
    int rb = 1;
    while (kvar) {
        printf("Redni broj: %d\nRegistracija: %s\nOpis: %s\nDatum: %s\nCijena: %.2f\nStatus: %s\n----------------\n",
            rb++, kvar->registracija, kvar->opis, kvar->datum, kvar->cijena, statusToString(kvar->status));
        kvar = kvar->next;
    }
}

void azurirajKvarPopravak(KvarPopravak* glava) {
    char reg[MAX_REG] = {0};
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

void obrisiKvarPopravak(KvarPopravak** glava) {
    char reg[MAX_REG] = {0};
    printf("Unesi registraciju vozila za brisanje: ");
    scanf("%11s", reg);
    KvarPopravak* prev = NULL;
    KvarPopravak* curr = *glava;
    while (curr) {
        if (strcmp(curr->registracija, reg) == 0) {
            if (prev) prev->next = curr->next;
            else *glava = curr->next;
            free(curr);
            globalBrojKvarova--; // 8. Smanjivanje globalnog broja kvarova
            printf("Kvar obrisan.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Kvar nije pronađen.\n");
}

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
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    } while (zamjena);
    printf("Kvarovi sortirani po datumu.\n");
}

void pretraziKvarPopravak(const KvarPopravak* glava) {
    char reg[MAX_REG] = {0};
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
        fwrite(kvar, sizeof(KvarPopravak) - sizeof(KvarPopravak*), 1, fp);
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
    // 20. Demonstracija fseek, ftell, rewind
    fseek(fp, 0, SEEK_END);
    long velicina = ftell(fp);
    rewind(fp);
    printf("Veličina datoteke je %ld bajtova\n", velicina);

    oslobodiMemoriju(glava);
    int broj = 0;
    fread(&broj, sizeof(int), 1, fp);
    globalBrojKvarova = broj; // 8. Sinkronizacija globalnog broja kvarova
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

void oslobodiMemoriju(KvarPopravak** glava) {
    while (*glava) {
        KvarPopravak* tmp = *glava;
        *glava = (*glava)->next;
        free(tmp);
    }
}
#include "header.h"

// 8. Definicija globalne varijable
int globalBrojKvarova = 0;

// 9. Primjer inline funkcije
inline int zbroji(int a, int b) { return a + b; }

const char* statusToString(StatusPopravka status) {
    switch (status) {
        case STATUS_CEKA_DIJELOVE: return "ČEKA DIJELOVE";
        case STATUS_POPRAVLJENO:   return "POPRAVLJENO";
        case STATUS_U_TOKU:        return "U TOKU";
        case STATUS_ODBIJENO:      return "ODBIJENO";
        default:                   return "NEPOZNATO";
    }
}

void dodajKvarPopravak(KvarPopravak** glava) {
    KvarPopravak* novi = (KvarPopravak*)malloc(sizeof(KvarPopravak));
    if (!novi) {
        perror("Greska pri alokaciji memorije");
        return;
    }
    printf("Unesi registraciju vozila: ");
    scanf("%11s", novi->registracija);
    printf("Unesi opis kvara: ");
    getchar();
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
    globalBrojKvarova++; // 8. Povećavanje globalnog broja kvarova
    printf("Kvar je dodan.\n");
}

void ispisiSveKvarove(const KvarPopravak* glava) {
    static int brojPoziva = 0; // 6. static lokalna varijabla
    brojPoziva++;
    printf("Funkcija ispisiSveKvarove je pozvana %d puta\n", brojPoziva);
    printf("Ukupno upisanih kvarova: %d\n", globalBrojKvarova); // 8. Korištenje globalne varijable
    const KvarPopravak* kvar = glava;
    int rb = 1;
    while (kvar) {
        printf("Redni broj: %d\nRegistracija: %s\nOpis: %s\nDatum: %s\nCijena: %.2f\nStatus: %s\n----------------\n",
            rb++, kvar->registracija, kvar->opis, kvar->datum, kvar->cijena, statusToString(kvar->status));
        kvar = kvar->next;
    }
}

void azurirajKvarPopravak(KvarPopravak* glava) {
    char reg[MAX_REG] = {0};
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

void obrisiKvarPopravak(KvarPopravak** glava) {
    char reg[MAX_REG] = {0};
    printf("Unesi registraciju vozila za brisanje: ");
    scanf("%11s", reg);
    KvarPopravak* prev = NULL;
    KvarPopravak* curr = *glava;
    while (curr) {
        if (strcmp(curr->registracija, reg) == 0) {
            if (prev) prev->next = curr->next;
            else *glava = curr->next;
            free(curr);
            globalBrojKvarova--; // 8. Smanjivanje globalnog broja kvarova
            printf("Kvar obrisan.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Kvar nije pronađen.\n");
}

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
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    } while (zamjena);
    printf("Kvarovi sortirani po datumu.\n");
}

void pretraziKvarPopravak(const KvarPopravak* glava) {
    char reg[MAX_REG] = {0};
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
        fwrite(kvar, sizeof(KvarPopravak) - sizeof(KvarPopravak*), 1, fp);
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
    // 20. Demonstracija fseek, ftell, rewind
    fseek(fp, 0, SEEK_END);
    long velicina = ftell(fp);
    rewind(fp);
    printf("Veličina datoteke je %ld bajtova\n", velicina);

    oslobodiMemoriju(glava);
    int broj = 0;
    fread(&broj, sizeof(int), 1, fp);
    globalBrojKvarova = broj; // 8. Sinkronizacija globalnog broja kvarova
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

void oslobodiMemoriju(KvarPopravak** glava) {
    while (*glava) {
        KvarPopravak* tmp = *glava;
        *glava = (*glava)->next;
        free(tmp);
    }
}
