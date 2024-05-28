#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ANGAJATI 3
#define NUM_SETS 10

// Structura pentru o carte
typedef struct {
    int numar_pagini;
} Carte;

// Functie pentru generarea de numere aleatoare
int generare_numar_aleator(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Functie pentru calcularea sumelor cumulative ale paginilor
void calculeaza_sume_cumulative(int *sume_cumulative, Carte *carti, int numar_carti) {
    sume_cumulative[0] = carti[0].numar_pagini;
    for (int i = 1; i < numar_carti; i++) {
        sume_cumulative[i] = sume_cumulative[i - 1] + carti[i].numar_pagini;
    }
}

// Functie pentru a calcula suma paginilor intr-un interval
int suma_pagini_interval(int *sume_cumulative, int start, int end) {
    if (start == 0) {
        return sume_cumulative[end];
    } else {
        return sume_cumulative[end] - sume_cumulative[start - 1];
    }
}

// Functie pentru a gasi punctele optime de taiere
void imparte_cartile(Carte *carti, int numar_carti, int limite[NUM_ANGAJATI - 1]) {
    int sume_cumulative[numar_carti];
    calculeaza_sume_cumulative(sume_cumulative, carti, numar_carti);

    int min_diferenta = 1e9;
    int solutii[NUM_ANGAJATI - 1];

    void backtrack(int idx, int angajat, int start) {
        if (angajat == NUM_ANGAJATI - 1) {
            int suma = suma_pagini_interval(sume_cumulative, start, numar_carti - 1);
            int max_suma = suma;
            int min_suma = suma;
            for (int i = 0; i < NUM_ANGAJATI - 1; i++) {
                int suma_angajat = suma_pagini_interval(sume_cumulative, (i == 0 ? 0 : limite[i - 1] + 1), limite[i]);
                if (suma_angajat > max_suma) {
                    max_suma = suma_angajat;
                }
                if (suma_angajat < min_suma) {
                    min_suma = suma_angajat;
                }
            }
            int diferenta = max_suma - min_suma;
            if (diferenta < min_diferenta) {
                min_diferenta = diferenta;
                for (int i = 0; i < NUM_ANGAJATI - 1; i++) {
                    solutii[i] = limite[i];
                }
            }
            return;
        }
        for (int i = start; i < numar_carti - (NUM_ANGAJATI - 1 - angajat); i++) {
            limite[angajat] = i;
            backtrack(idx + 1, angajat + 1, i + 1);
        }
    }

    backtrack(0, 0, 0);

    for (int i = 0; i < NUM_ANGAJATI - 1; i++) {
        limite[i] = solutii[i];
    }
}

int main() {
    srand(time(NULL)); // Inițializare seed pentru funcția rand()

    for (int set = 0; set < NUM_SETS; set++) {
        printf("Setul %d:\n", set + 1);
        // Generare un numar random de carti
        int numar_carti = generare_numar_aleator(5, 15); // Numar mic de carti pentru variatie
        printf("Numarul de carti: %d\n", numar_carti);

        // Alocare de memorie pentru vectorul de carti
        Carte *carti = (Carte *)malloc(numar_carti * sizeof(Carte));

        // Generare pentru fiecare carte a unui numar random de pagini mare (peste 1000)
        printf("Numarul de pagini pentru fiecare carte:\n");
        for (int i = 0; i < numar_carti; i++) {
            carti[i].numar_pagini = generare_numar_aleator(1000, 2000); // Numar mare de pagini între 1000 și 2000
            printf("Carte %d: %d pagini\n", i + 1, carti[i].numar_pagini);
        }
        printf("\n");

        // Alocare pagini pentru fiecare angajat
        int limite[NUM_ANGAJATI - 1] = {0};
        imparte_cartile(carti, numar_carti, limite);

        // Afisam sarcina fiecarui angajat
        printf("Sarcina fiecarui angajat:\n");
        for (int i = 0; i < NUM_ANGAJATI; i++) {
            printf("Angajat %d: ", i + 1);
            int start = (i == 0) ? 0 : limite[i - 1] + 1;
            int end = (i == NUM_ANGAJATI - 1) ? numar_carti - 1 : limite[i];
            for (int j = start; j <= end; j++) {
                printf("%d ", carti[j].numar_pagini);
            }
            printf("\n");
        }
        printf("\n");

        // Eliberare memorie alocata pentru vectorul de carti
        free(carti);
    }

    return 0;
}
