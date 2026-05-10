#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
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
    struct Nod* stanga;
    struct Nod* dreapta;
};
typedef struct Nod Nod;

void adaugaFilmInArbore(Nod** radacina, Film film)
{
    if (*radacina != NULL)
    {
        //comparam dupa id
        if ((*radacina)->info.id < film.id)
        {
            adaugaFilmInArbore(&(*radacina)->dreapta, film);
        }
        else
        {
            adaugaFilmInArbore(&(*radacina)->stanga, film);
        }
    }
    else
    {
        //devine radacina
        //alocam spatiu
        Nod* nod = (Nod*)malloc(sizeof(Nod));
        nod->info = film;
        nod->dreapta = NULL;
        nod->stanga = NULL;
        *radacina = nod;
    }
}

Nod* citireArboreDeFilmeDinFisier(const char* numeFisier)
{
    FILE* file = fopen(numeFisier, "r");
    Nod* radacina = NULL;
    if (file)
    {
        while (!feof(file))
        {
            Film film = citireFilmDinFisier(file);
            adaugaFilmInArbore(&radacina, film);
        }
        fclose(file);
    }
    return radacina;
}

void afisareFilmeDinArboreInordine(Nod* radacina)
{
    if (radacina!=NULL)
    {
        afisareFilmeDinArboreInordine(radacina->stanga);  //tot subarborele stang
        afisareFilm(radacina->info);
        afisareFilmeDinArboreInordine(radacina->dreapta); // tot subarborele drept
    }
}

void afisareFilmeDinArborePreordine(Nod* radacina)
{
    if (radacina != NULL)
    {
        afisareFilm(radacina->info);
        afisareFilmeDinArborePreordine(radacina->stanga);
        afisareFilmeDinArborePreordine(radacina->dreapta);

    }
}
void afisareFilmeDinArborePostordine(Nod* radacina)
{
    if (radacina != NULL)
    {
        afisareFilmeDinArborePostordine(radacina->stanga);
        afisareFilmeDinArborePostordine(radacina->dreapta);
        afisareFilm(radacina->info);
    }
}

void dezalocareArboreDeFilme(Nod**radacina) {
    if (*radacina)
    {
        dezalocareArboreDeFilme(&(*radacina)->stanga); //dezalocam subarbore stang
        dezalocareArboreDeFilme(&(*radacina)->dreapta);//dezalocam subarbore drept

        if ((*radacina)->info.titlu != NULL)
        {
            free((*radacina)->info.titlu);
        }
        if ((*radacina)->info.regizor != NULL)
        {
            free((*radacina)->info.regizor);
        }

        free(*radacina);
        *radacina = NULL;
    }
}

Film getFilmById(Nod* radacina, int id)
{
    if (radacina) {
        if (radacina->info.id==id) //e radacina
        {
            Film f = radacina->info;
            f.titlu=malloc(strlen(radacina->info.titlu)+1);
            strcpy(f.titlu,radacina->info.titlu);

            f.regizor=malloc(strlen(radacina->info.regizor)+1);
            strcpy(f.regizor,radacina->info.regizor);

            return f;
        }

        if (radacina->info.id < id) 
        {
            return getFilmById(radacina->dreapta, id);
        }

        if (radacina->info.id > id) 
        {
            return getFilmById(radacina->stanga, id);
        }
    }
    //daca nu exista

    Film f;
    f.id = -1;
    f.titlu = NULL;
    f.regizor = NULL;
    return f;
}
int determinaNumarNoduri(Nod*radacina) {
    if (radacina)
    {
        return 1 + determinaNumarNoduri(radacina->stanga)+determinaNumarNoduri(radacina->dreapta);
    }
	return 0;
}

int calculeazaInaltimeArbore(Nod*radacina) {
    if (radacina)
    {
       int inaltimeStanga = calculeazaInaltimeArbore(radacina->stanga);
       int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dreapta);

       return 1 + max(inaltimeStanga, inaltimeDreapta);
    }
	return 0;
}
int calculeazaDurataTotala(Nod* radacina)
{
    if (radacina)
    {
        return calculeazaDurataTotala(radacina->stanga) + calculeazaDurataTotala(radacina->dreapta) + radacina->info.durata;
    }
    return 0;
}
int calculeazaDuarataFilmelorUnuiRegizor(Nod* radacina, const char* regizor) 
{
    int suma;
    if (radacina)
    {
        suma = calculeazaDuarataFilmelorUnuiRegizor(radacina->stanga,regizor) + calculeazaDuarataFilmelorUnuiRegizor(radacina->dreapta,regizor);

        //daca e si radacina
        if (strcmp(radacina->info.regizor, regizor) == 0)
        {
            suma += radacina->info.durata;
        }
        return suma;
    }
    return 0;
}
int main() {

    Nod* radacina = citireArboreDeFilmeDinFisier("filme_arbore.txt");

    printf("afisare inordine:\n");
    afisareFilmeDinArboreInordine(radacina);

    Film f = getFilmById(radacina, 7);

    if (f.id != -1) 
    {
        printf("\nfilm cautat:\n");
        afisareFilm(f);

        free(f.titlu);
        free(f.regizor);
    }
    else 
    {
        printf("Nu exista film cu acest id!\n");
    }

    printf("Nr noduri: %d\n", determinaNumarNoduri(radacina));
    printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
    printf("Durata totala: %d\n", calculeazaDurataTotala(radacina));
    printf("Duarata totala filme Christopher Nolan: %d\n", calculeazaDuarataFilmelorUnuiRegizor(radacina, "Christopher Nolan"));

    dezalocareArboreDeFilme(&radacina);
}