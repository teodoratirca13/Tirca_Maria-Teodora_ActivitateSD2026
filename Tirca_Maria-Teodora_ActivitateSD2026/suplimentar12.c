#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct StructuraCarte
{
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
	unsigned char categorie;
};
typedef struct StructuraCarte Carte;
Carte citireCarteDinFisier(FILE* file)
{
	char linie[100];
	fgets(linie, 100, file);

	char separator[3] = ",\n";
	Carte c;

	c.id = atoi(strtok(linie, separator));
	c.nrPagini = atoi(strtok(NULL, separator));
	c.pret = atof(strtok(NULL, separator));

	char* aux;
	aux = strtok(NULL, separator);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);

	aux = strtok(NULL, separator);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);

	c.categorie = strtok(NULL, separator)[0];
	return c;
}
void afisareCarte(Carte carte)
{
	printf("Id: %d\n", carte.id);
	printf("Nr. pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Categorie: %c\n\n", carte.categorie);
}
struct Nod
{
	Carte info;
	struct Nod* next;
};
typedef struct Nod Nod;

void enqueue(Nod** cap, Carte carte)
{
	//adaugam la final
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = carte;
	nod->next = NULL;

	if (*cap == NULL) //e goala
	{
		*cap = nod;
	}
	else
	{
		Nod* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = nod;
	}
}
Carte dequeue(Nod** cap)
{
	//extragem de la inceput
	Carte carte;
	carte.id = -1;
	if (*cap != NULL)
	{
		//avem ce extrage
		carte = (*cap)->info;
		Nod* p = *cap;
		*cap = (*cap)->next;
		free(p);
	}
	return carte;
}
Nod* citireCoadaDeCartiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Nod* queue = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			enqueue(&queue, carte);
		}
		fclose(file);
	}
	return queue;
}
void afisareCoada(Nod* cap)
{
	while (cap)
	{
		afisareCarte(cap->info);
		cap = cap->next;
	}
}
void dezalocareCoada(Nod** queue)
{
	/*while ((*queue) != NULL)
	{
		Nod* p = *queue;
		*queue = (*queue)->next;

		if (p->info.autor)
		{
			free(p->info.autor);
		}
		if (p->info.titlu)
		{
			free(p->info.titlu);
		}
		free(p);
	}*/

	while (*queue)
	{
		Carte carte = dequeue(queue);
		//acolo avem free(p_
		if (carte.titlu)
			free(carte.titlu);
		if (carte.autor)
			free(carte.autor);
	}
}
Carte getCarteByIDStack(Nod** queue, int id)
{
	Nod* aux = NULL;
	Carte rezultat;
	rezultat.id = -1;
	while (*queue)
	{
		Carte carte = dequeue(queue);
		if (carte.id == id)
		{
			rezultat = carte;
			rezultat.autor = (char*)malloc(strlen(carte.autor) + 1);
			strcpy(rezultat.autor, carte.autor);
			rezultat.titlu = (char*)malloc(strlen(carte.titlu) + 1);
			strcpy(rezultat.titlu, carte.titlu);
		}
		enqueue(&aux, carte);
	}
	//refacem coada
	while (aux)
	{
		enqueue(queue, dequeue(&aux));
	}
	return rezultat;
}
int main()
{
	Nod* coada = citireCoadaDeCartiDinFisier("carti.txt");

	printf("COADA INITIALA:\n\n");
	afisareCoada(coada);

	printf("=====================================\n");

	Carte c = getCarteByIDStack(&coada, 2);
	if (c.id != -1)
	{
		printf("Cartea gasita dupa ID:\n\n");
		afisareCarte(c);

		free(c.titlu);
		free(c.autor);
	}
	else
	{
		printf("Nu a fost gasita cartea cu ID-ul cautat\n");
	}

	printf("=====================================\n");
	printf("Coada dupa cautare:\n\n");
	afisareCoada(coada);

	dezalocareCoada(&coada);

	return 0;
}