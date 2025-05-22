#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>      // 7. Organizacija izvornog koda
#include <stdlib.h>     // 7. Organizacija izvornog koda
#include <string.h>     // 7. Organizacija izvornog koda

// 8. extern deklaracija
extern int globalBrojKvarova;

// 9. makro konstante za velicine polja
#define MAX_REG 12
#define MAX_OPIS 256

// 4. typedef, enum za status popravka
typedef enum {
    STATUS_CEKA_DIJELOVE,   // 4. enum
    STATUS_POPRAVLJENO,     // 4. enum
    STATUS_U_TOKU,          // 4. enum
    STATUS_ODBIJENO         // 4. enum
} StatusPopravka;

// 13. 3. Složeni tipovi, 4. typedef, 16. Dinamička memorija (povezani popis)
typedef struct kvar_popravak {
    char registracija[MAX_REG];      // 9. koristenja makroa
    char opis[MAX_OPIS];             // 9. koristenja makroa
    char datum[11];             // 2. char[], 15. Statički zauzeto polje
    float cijena;               // 2. float
    StatusPopravka status;      // 4. enum
    struct kvar_popravak* next; // 12. Pokazivač, 16. Dinamička memorija
} KvarPopravak;                 // 3. Složeni tip, 4. typedef

// 5. Imenovanje identifikatora (camelCase, snake_case dosljedno)
// 7. Organizacija izvornog koda (deklaracije u headeru)
void dodajKvarPopravak(KvarPopravak** glava);
void ispisiSveKvarove(const KvarPopravak* glava);
void azurirajKvarPopravak(KvarPopravak* glava);
void obrisiKvarPopravak(KvarPopravak** glava);
void sortirajKvarove(KvarPopravak** glava);
void pretraziKvarPopravak(const KvarPopravak* glava);
void spremiKvarove(const KvarPopravak* glava, const char* filename);
void ucitajKvarove(KvarPopravak** glava, const char* filename);
void oslobodiMemoriju(KvarPopravak** glava);
const char* statusToString(StatusPopravka status);
