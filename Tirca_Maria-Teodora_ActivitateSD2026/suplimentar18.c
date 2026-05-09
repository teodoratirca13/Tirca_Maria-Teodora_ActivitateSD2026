#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct StructuraFilm
{
    int id;
    int durata;
    float rating;
    char* titlu;
    char* regizor;
    unsigned char categorie;
};
typedef struct StructuraFilm Film;

void afisareFilm(Film film)
{
    printf("Id: %d\n", film.id);
    printf("Durata: %d minute\n", film.durata);
    printf("Rating: %.2f\n", film.rating);
    printf("Titlu: %s\n", film.titlu);
    printf("Regizor: %s\n", film.regizor);
    printf("Categorie: %c\n\n", film.categorie);
}

Film citireFilmDinFisier(FILE* file)
{
    Film f;
    char linie[100];
    char separator[3] = ",\n";
    fgets(linie, 100, file);

    f.id = atoi(strtok(linie, separator));
    f.durata = atoi(strtok(NULL, separator));
    f.rating = atof(strtok(NULL, separator));

    char* aux1 = strtok(NULL, separator);
    f.titlu = (char*)malloc(sizeof(char) * (strlen(aux1) + 1));
    strcpy(f.titlu, aux1);

    char* aux2 = strtok(NULL, separator);
    f.regizor = (char*)malloc(sizeof(char) * (strlen(aux2) + 1));
    strcpy(f.regizor, aux2);

    f.categorie = strtok(NULL, separator)[0];
    return f;
}
struct Heap {
    int lungime;
    Film* vector;
    int nrElemente;
};
typedef struct Heap Heap;

Heap initializareHeap(int lungime) {
    Heap h;
    h.lungime=lungime;
    h.nrElemente=0;
    h.vector=(Film*)malloc(sizeof(Film)*lungime);
    return h;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
    int fiuSt=2*pozitieNod+1;
    int fiuDr=2*pozitieNod+2;
    int pMax=pozitieNod;

    if(fiuSt<heap.nrElemente && heap.vector[pMax].rating<heap.vector[fiuSt].rating) {
        pMax=fiuSt;
    }

    if (fiuDr<heap.nrElemente &&
        heap.vector[pMax].rating<heap.vector[fiuDr].rating) {
        pMax=fiuDr;
    }

    if (pMax!=pozitieNod) {
        Film aux=heap.vector[pMax];
        heap.vector[pMax]=heap.vector[pozitieNod];
        heap.vector[pozitieNod]=aux;

        filtreazaHeap(heap,pMax);
    }
}

Heap citireHeapDeFilmeDinFisier(const char* numeFisier) {
    FILE* f=fopen(numeFisier,"r");

    Heap h=initializareHeap(8);
    int i=0;

    while (!feof(f)) {
        Film film=citireFilmDinFisier(f);
        h.vector[i]=film;
        i++;
    }

    h.nrElemente = i;
    fclose(f);
    for (int j=(h.nrElemente-2)/2;j>=0;j--)
    {
        filtreazaHeap(h, j);
    }
    return h;
}

void afisareHeap(Heap heap) {
    for (int i=0; i<heap.nrElemente;i++) 
    {
        afisareFilm(heap.vector[i]);
    }
}

void afiseazaHeapAscuns(Heap heap) {
    for (int i=heap.nrElemente;i<heap.lungime;i++)
    {
        afisareFilm(heap.vector[i]);
    }
}

Film extrageFilm(Heap* heap) {
    Film film;
    film.id=-1;
    film.titlu=NULL;
    film.regizor=NULL;

    if (heap->nrElemente !=0) 
    {
        film = heap->vector[0];

        heap->vector[0] = heap->vector[heap->nrElemente - 1];
        heap->vector[heap->nrElemente - 1]=film;
        heap->nrElemente--;

        for (int i=(heap->nrElemente-2)/2;i>=0;i--) {
            filtreazaHeap(*heap, i);
        }
    }
    return film;
}

void dezalocareHeap(Heap* heap) {
    for (int i=0;i<heap->lungime;i++) {
        free(heap->vector[i].titlu);
        free(heap->vector[i].regizor);
    }

    free(heap->vector);
    heap->lungime=0;
    heap->nrElemente=0;
    heap->vector=NULL;
}

int main() {
    Heap h=citireHeapDeFilmeDinFisier("filme.txt");
    afisareHeap(h);

    printf("\n\nExtrageri:\n");

    afisareFilm(extrageFilm(&h));
    afisareFilm(extrageFilm(&h));
    afisareFilm(extrageFilm(&h));

    printf("\n\nHeap ascuns:\n");
    afiseazaHeapAscuns(h);
    dezalocareHeap(&h);

    return 0;
}