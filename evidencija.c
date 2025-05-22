#define _CRT_SECURE_NO_WARNINGS
#include "header.h"

// 8. definicija globalne varijable
int globalBrojKvarova = 0; 

// 9. Jednostavna funkcija (mogla bi biti inline)
const char* statusToString(StatusPopravka status) {
    switch (status) {
    case STATUS_CEKA_DIJELOVE: 
		return "CEKA DIJELOVE";
    case STATUS_POPRAVLJENO:   
		return "POPRAVLJENO";
    case STATUS_U_TOKU:        
		return "U TOKU";
    case STATUS_ODBIJENO:      
		return "ODBIJENO";
    default:                   
		return "NEPOZNATO";
    }
}

// 1. Create (dodavanje), 12. Pokazivači, 16. malloc
void dodajKvarPopravak(KvarPopravak** glava) {
    KvarPopravak* novi = (KvarPopravak*)malloc(sizeof(KvarPopravak)); // 16. malloc()
    if (!novi) {                                                     // 14. Zaštita parametara
        perror("Greska pri alokaciji memorije");                     // 22. perror()
        return;
    }
    printf("\nUnesi registraciju vozila: ");
    scanf("%11s", novi->registracija);
    printf("Unesi opis kvara: ");
    getchar(); // flush
    fgets(novi->opis, sizeof(novi->opis), stdin);
    novi->opis[strcspn(novi->opis, "\n")] = 0;
    printf("Unesi datum (dd.mm.gggg): ");
    scanf("%10s", novi->datum);
    printf("Unesi cijenu popravka (EUR): ");
    scanf("%f", &novi->cijena);
    printf("Status (0-CEKA DIJELOVE, 1-POPRAVLJENO, 2-U TOKU, 3-ODBIJENO): ");
    int status;
    scanf("%d", &status);
    novi->status = (StatusPopravka)status; // 4. enum
    novi->next = *glava;
    *glava = novi;
	globalBrojKvarova++;
    printf("Kvar je dodan.\n");
}

// 1. Read (ispis), 12. Pokazivači
void ispisiSveKvarove(const KvarPopravak* glava) {
	static int brojPoziva = 0; // 6. static lokalna varijabla
	brojPoziva++;
	printf("Fubkcija je pozvana %d puta\n", brojPoziva);
	printf("Ukupno upisanih kvarova %d\n", globalBrojKvarova); // 8.
    const KvarPopravak* kvar = glava;
    int rb = 1;
    while (kvar) {
        printf("\n----------------\nRedni broj: %d\nRegistracija: %s\nOpis: %s\nDatum: %s\nCijena(EUR): %.2f\nStatus: %s\n----------------\n",
            rb++, kvar->registracija, kvar->opis, kvar->datum, kvar->cijena, statusToString(kvar->status)); // 9. Poziv jednostavne funkcije
        kvar = kvar->next;
    }
}

// 1. Update (ažuriranje), 12. Pokazivači
void azurirajKvarPopravak(KvarPopravak* glava) {
    char reg[12] = { 0 }; // 2. char[], 15. Statički zauzeto polje
    printf("\nUnesi registraciju vozila za azuriranje: ");
    scanf("%11s", reg);
    for (KvarPopravak* kvar = glava; kvar; kvar = kvar->next) {
        if (strcmp(kvar->registracija, reg) == 0) {
            printf("Unesi novi opis: ");
            getchar();
            fgets(kvar->opis, sizeof(kvar->opis), stdin);
            kvar->opis[strcspn(kvar->opis, "\n")] = 0;
            printf("Unesi novu cijenu (EUR): ");
            scanf("%f", &kvar->cijena);
            printf("Unesi novi status (0-CEKA DIJELOVE, 1-POPRAVLJENO, 2-U TOKU, 3-ODBIJENO): ");
            int status;
            scanf("%d", &status);
            kvar->status = (StatusPopravka)status;
            printf("Kvar azuriran.\n");
            return;
        }
    }
    printf("Kvar nije pronadjen.\n");
}

// 1. Delete (brisanje), 12. Pokazivači, 18. free()
void obrisiKvarPopravak(KvarPopravak** glava) {
    char reg[12] = { 0 }; // 2. char[], 15. Statički zauzeto polje
    printf("\nUnesi registraciju vozila za brisanje: ");
    scanf("%11s", reg);
    KvarPopravak* prev = NULL;
    KvarPopravak* curr = *glava;
    while (curr) {
        if (strcmp(curr->registracija, reg) == 0) {
            if (prev) prev->next = curr->next;
            else *glava = curr->next;
            free(curr); // 18. free()
			globalBrojKvarova--;
            printf("Kvar obrisan.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("\nKvar nije pronadjen.\n");
}

// 23. Sortiranje (bubble sort za listu)
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
    printf("\nKvarovi sortirani po datumu.\n");
}

// 24. Pretraživanje (po registraciji)
void pretraziKvarPopravak(const KvarPopravak* glava) {
    char reg[12] = { 0 }; // 2. char[], 15. Statički zauzeto polje
    printf("\nUnesi registraciju za pretragu: ");
    scanf("%11s", reg);
    for (const KvarPopravak* kvar = glava; kvar; kvar = kvar->next) {
        if (strcmp(kvar->registracija, reg) == 0) {
            printf("Kvar pronadjen: %s, opis: %s\n", kvar->registracija, kvar->opis);
            return;
        }
    }
    printf("Nema kvara za tu registraciju.\n");
}

// 19. Datoteke (binarno spremanje/učitavanje), 14. Zaštita parametara, 22. perror()
void spremiKvarove(const KvarPopravak* glava, const char* filename) {
    FILE* fp = fopen(filename, "wb"); // 19. fopen()
    if (!fp) {
        perror("\nGreska pri otvaranju datoteke za spremanje");
        return;
    }
    int broj = 0;
    for (const KvarPopravak* kvar = glava; kvar; kvar = kvar->next) broj++;
    fwrite(&broj, sizeof(int), 1, fp); // 19. fwrite()
    for (const KvarPopravak* kvar = glava; kvar; kvar = kvar->next) {
        fwrite(kvar, sizeof(KvarPopravak) - sizeof(KvarPopravak*), 1, fp); // 15. Statički zauzeta polja
    }
    fclose(fp); // 19. fclose()
    printf("\nPodaci spremljeni u datoteku.\n");
}

void ucitajKvarove(KvarPopravak** glava, const char* filename) {
    FILE* fp = fopen(filename, "rb"); // 19. fopen()
    if (!fp) {
        perror("\nGreska pri otvaranju datoteke za ucitavanje");
        return;
    }
	fseek(fp, 0, SEEK_END);
	long velicina = ftell(fp); // 20. ftell
	rewind(fp);
	printf("Velicina datoteke je %1d bajtova\n", velicina);
    oslobodiMemoriju(glava); // 18. Oslobađanje stare memorije
    int broj = 0;
    fread(&broj, sizeof(int), 1, fp); // 19. fread()
    for (int i = 0; i < broj; ++i) {
        KvarPopravak* novi = (KvarPopravak*)malloc(sizeof(KvarPopravak)); // 16. malloc()
        if (!novi) {
            perror("\nGreska pri alokaciji memorije");
            fclose(fp);
            return;
        }
        fread(novi, sizeof(KvarPopravak) - sizeof(KvarPopravak*), 1, fp); // 15. Statički zauzeta polja
        novi->next = *glava;
        *glava = novi;
    }
    fclose(fp); // 19. fclose()
    printf("\nPodaci ucitani iz datoteke.\n");
}

// 18. Oslobađanje memorije, 17. free()
void oslobodiMemoriju(KvarPopravak** glava) {
    while (*glava) {
        KvarPopravak* tmp = *glava;
        *glava = (*glava)->next;
        free(tmp); // 17. free()
    }
}
