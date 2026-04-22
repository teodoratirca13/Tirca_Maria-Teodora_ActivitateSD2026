#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
struct Nod
{
    Film info;
    struct Nod* next;
};
typedef struct Nod Nod;
void pushStack(Nod** stiva, Film film)
{
    //adaugare la final

    Nod* nod = (Nod*)malloc(sizeof(Nod));
    nod->info = film;
    nod->next = NULL;

    if (*stiva == NULL)
    {
        *stiva = nod;
    }
    else
    {
        Nod* p = *stiva;
        while (p->next)
        {
            p = p->next;
        }
        p->next = nod;
    }
}
Film popStack(Nod** stiva)
{
    //extragere de la final
    Film film;
    film.id = -1;
    film.regizor = NULL;
    film.titlu = NULL;

    if (*stiva == NULL)
    {
        return film;
    }
    if ((*stiva)->next == NULL) //avem doar un nod;
    {
        film = (*stiva)->info;
        Nod* aux = *stiva;
        free(aux);

        *stiva = NULL;
    }
    else
    {
        Nod* p = *stiva;
        while (p->next->next)
        {
            p = p->next;
        }
        film = p->next->info;
        Nod* aux = p->next;
        free(aux);
        p->next = NULL;

    }

    return film;
}
char emptyStack(Nod* stiva)
{
    return stiva == NULL;
}
Nod* citireStivaDinFisier(const char* numeFisier)
{
    FILE* file = fopen(numeFisier, "r");
    Nod* stiva = NULL;
    if (file)
    {
        while (!feof(file))
        {
            Film film = citireFilmDinFisier(file);
            pushStack(&stiva, film);
        }
        fclose(file);
    }
    return stiva;
}
void afisareStiva(Nod* stiva)
{
    while (stiva)
    {
        afisareFilm(stiva->info);
        stiva = stiva->next;
    }
}
void dezalocareStiva(Nod** stiva)
{
    while (!emptyStack(*stiva))
    {
        Film film = popStack(stiva);
        if (film.regizor)
            free(film.regizor);
        if (film.titlu)
            free(film.titlu);
    }
}
Film getFilmById(Nod** stiva, int id)
{
    Film rezultat;
    rezultat.id = -1;
    rezultat.regizor = NULL;
    rezultat.titlu = NULL;
    Nod* aux = NULL;
    while (*stiva != NULL)
    {
        Film film = popStack(stiva);

        if (film.id == id && rezultat.id == -1)
        {
            rezultat = film;

            rezultat.titlu = (char*)malloc(strlen(film.titlu) + 1);
            strcpy(rezultat.titlu, film.titlu);

            rezultat.regizor = (char*)malloc(strlen(film.regizor) + 1);
            strcpy(rezultat.regizor, film.regizor);
        }

        pushStack(&aux, film);
    }

    while (aux != NULL)
    {
        Film film = popStack(&aux);
        pushStack(stiva, film);
    }

    return rezultat;
}
int main()
{
    Nod* stiva = citireStivaDinFisier("filme.txt");
    afisareStiva(stiva);

    Film filmCautat = getFilmById(&stiva, 3);

    if (filmCautat.id != -1)
    {
        printf("\nFilmul gasit este:\n\n");
        afisareFilm(filmCautat);

        free(filmCautat.titlu);
        free(filmCautat.regizor);
    }
    else
    {
        printf("\nNu exista film cu acest id!\n");
    }

    dezalocareStiva(&stiva);
    return 0;
}