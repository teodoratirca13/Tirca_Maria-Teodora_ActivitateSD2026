#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

struct Carte
{
    int id;
    int nrPagini;
    char* titlu;
    char* autor;
    float pret;
    char categorie;
};

struct Carte initializare(int id, int nrPagini, char* titlu, char* autor, float pret, char categorie)
{
    struct Carte c;
    c.id = id;
    c.nrPagini = nrPagini;
    c.pret = pret;
    c.categorie = categorie;
    c.titlu = (char*)malloc(sizeof(char) * (strlen(titlu) + 1));
    strcpy(c.titlu, titlu);
    c.autor = (char*)malloc(sizeof(char) * (strlen(autor) + 1));
    strcpy(c.autor, autor);

    return c;
}

void afisare(struct Carte c)
{
    printf("Id: %d\n", c.id);
    printf("Numar pagini: %d\n", c.nrPagini);
    printf("Titul: %s\n", c.titlu);
    printf("Autor: %s\n", c.autor);
    printf("Pret: %5.2f\n", c.pret);
    printf("Categorie: %c\n\n", c.categorie);
}

void modificaDate(struct Carte* c, char* titluNou, char* autorNou)
{
    free((*c).autor);
    c->autor= (char*)malloc(sizeof(char) * (strlen(autorNou) + 1));
    strcpy(c->autor, autorNou);

    free((*c).titlu);
    c->titlu = (char*)malloc(sizeof(char) * (strlen(titluNou) + 1));
    strcpy(c->titlu, titluNou);
}

void dezalocare(struct Carte* c)
{
    if (c->autor != NULL)
    {
        free(c->autor);
        c->autor = NULL;
    }
    if (c->titlu != NULL)
    {
        free(c->titlu);
        c->titlu = NULL;
    }
}
bool titluContineCuvant(struct Carte c, char* cuvant)
{
    if (strstr(c.titlu, cuvant) != NULL)
        return true;
    return false;
}
bool comparaDupaPagini(struct Carte c1, struct Carte c2)
{
    if (c1.nrPagini > c2.nrPagini)
        return true;
    return false;
}
bool esteCategorie(struct Carte c, char categorie)
{
    if (c.categorie == categorie)
        return true;
    return false;
}
int main()
{
    struct Carte c1;
    struct Carte c2;

    c1 = initializare(1, 432, "Mandrie si prejudecata", "Jane Austen", 0.20f, 'L');
    c2 = initializare(2, 384, "Unde canta racii", "Delia Owens", 0.22f, 'L');

    printf("====Afisare carti initiale:====\n");
    afisare(c1);
    afisare(c2);

    modificaDate(&c1, "Pe aripile vantului", "Margaret Mitchell");
    printf("====Dupa modificare c1:====\n");
    afisare(c1);

    if (comparaDupaPagini(c1, c2))
        printf("c1 are mai multe pagini decat c2.\n");
    else printf("c1 nu are mai multe pagini decat c2.\n");

    if (esteCategorie(c2, 'A'))
        printf("c2 are categoria A.\n");
    else printf("c2 nu are categoria A.\n");

    if (titluContineCuvant(c1, "vantului"))
        printf("Titlul lui c1 contine cuvantul cautat.\n");
    else printf("Titlul lui c1 nu contine cuvantul cautat.\n");
	return 0;
}
