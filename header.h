#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -> 4. typedef, enum za status popravka
typedef enum {
    STATUS_CEKA_DIJELOVE,
    STATUS_POPRAVLJENO,
    STATUS_U_TOKU,
    STATUS_ODBIJENO
} StatusPopravka;

// -> 3. Složeni tipovi, 4. typedef, 16. Dinamička memorija (povezani popis)
typedef struct kvar_popravak {
    char registracija[12];
    char opis[256];
    char datum[11];
    float cijena;
    StatusPopravka status;
    struct kvar_popravak* next;
} KvarPopravak;

// Deklaracije funkcija (-> 7. Organizacija izvornog koda)
void dodajKvarPopravak(KvarPopravak** glava);
void ispisiSveKvarove(const KvarPopravak* glava);
void azurirajKvarPopravak(KvarPopravak* glava);
void obrisiKvarPopravak(KvarPopravak** glava);
void sortirajKvarove(KvarPopravak** glava);
void pretraziKvarPopravak(const KvarPopravak* glava);
void spremiKvarove(const KvarPopravak* glava, const char* filename);
void ucitajKvarove(KvarPopravak** glava, const char* filename);
void oslobodiMemoriju(KvarPopravak** glava);
