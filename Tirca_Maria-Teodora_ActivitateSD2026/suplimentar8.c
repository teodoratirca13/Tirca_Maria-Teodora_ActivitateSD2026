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

struct Nod
{
	Carte info;
	struct Nod* next;
};
typedef struct Nod Nod;

void afisareCarte(Carte carte)
{
	printf("Id: %d\n", carte.id);
	printf("Nr. pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Categorie: %c\n\n", carte.categorie);
}

Carte citireCarteDinFisier(FILE* file)
{
	Carte c;
	char linie[100];
	char separator[3] = ",\n";

	fgets(linie, 100, file);

	c.id = atoi(strtok(linie, separator));
	c.nrPagini = atoi(strtok(NULL, separator));
	c.pret = atof(strtok(NULL, separator));

	char* aux;
	aux = strtok(NULL, separator);
	c.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(c.titlu, aux);

	aux= strtok(NULL, separator);
	c.autor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(c.autor, aux);

	c.categorie = (strtok(NULL, separator))[0];
	return c;
}

void adaugaLaFinal(Nod** cap, Carte carteNoua)
{
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = carteNoua;
	nod->next = NULL;

	if ((*cap) != NULL)
	{
		Nod* p = *cap;
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = nod;
	}
	else
	{
		*cap = nod;
	}
}

void adaugaLaInceput(Nod** cap, Carte carteNoua)
{
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = carteNoua;
	nod->next = *cap;
	*cap = nod;
}

void adaugaPePozitie(Nod** cap, Carte carteNoua, int poz)
{
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = carteNoua;
	nod->next = NULL;

	if (*cap == NULL || poz <= 0)
	{
		nod->next = *cap;
		*cap = nod;
	}
	else
	{
		Nod* p = *cap;
		int k = 0;

		while (p->next != NULL && k < poz - 1)
		{
			p = p->next;
			k++;
		}

		nod->next = p->next;
		p->next = nod;
	}
}

Nod* citireListaDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Nod* cap = NULL;

	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			adaugaLaFinal(&cap, carte);
			//adaugaLaInceput(&cap, carte);
		}
		fclose(file);
	}

	return cap;
}

void afisareLista(Nod* cap)
{
	while (cap != NULL)
	{
		afisareCarte(cap->info);
		cap = cap->next;
	}
}

void dezalocare(Nod** cap)
{
	while ((*cap) != NULL)
	{
		Nod* p = *cap;
		(*cap) = p->next;

		if (p->info.titlu != NULL)
		{
			free(p->info.titlu);
		}
		if (p->info.autor != NULL)
		{
			free(p->info.autor);
		}

		free(p);
	}
}
int main()
{
	Nod* cap = citireListaDinFisier("carti.txt");

	afisareLista(cap);

	dezalocare(&cap);

	return 0;
}