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
float pretMediu(Nod* cap)
{
	float suma = 0;
	int contor = 0;

	while (cap != NULL)
	{
		suma += cap->info.pret;
		contor++;
		cap = cap->next;
	}

	if (contor > 0)
	{
		return suma / contor;
	}
	return 0;
}

Carte copiazaCarte(Carte c)
{
	Carte rez;
	rez = c;

	rez.titlu = (char*)malloc(strlen(c.titlu) + 1);
	strcpy(rez.titlu, c.titlu);

	rez.autor = (char*)malloc(strlen(c.autor) + 1);
	strcpy(rez.autor, c.autor);

	return rez;
}

Nod* copiazaCartiScumpe(Nod* cap, float prag)
{
	Nod* rezultat = NULL;

	while (cap != NULL)
	{
		if (cap->info.pret > prag)
		{
			adaugaLaInceput(&rezultat, copiazaCarte(cap->info));
		}
		cap = cap->next;
	}

	return rezultat;
}

void stergeCartiCategorie(Nod** cap, const char categorie)
{
	while ((*cap) != NULL && (*cap)->info.categorie == categorie)
	{
		Nod* p = *cap;
		*cap = p->next;

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

	if ((*cap) != NULL)
	{
		Nod* p = *cap;

		while (p != NULL)
		{
			while (p->next != NULL && p->next->info.categorie != categorie)
			{
				p = p->next;
			}

			if (p->next != NULL)
			{
				Nod* aux = p->next;
				p->next = aux->next;

				if (aux->info.titlu != NULL)
				{
					free(aux->info.titlu);
				}
				if (aux->info.autor != NULL)
				{
					free(aux->info.autor);
				}

				free(aux);
			}
			else
			{
				p = p->next;
			}
		}
	}
}

void sortareListaDupaPret(Nod* cap)
{
	if (cap == NULL)
	{
		return;
	}

	for (Nod* i = cap; i->next != NULL; i = i->next)
	{
		for (Nod* j = i->next; j != NULL; j = j->next)
		{
			if (i->info.pret > j->info.pret)
			{
				Carte aux = i->info;
				i->info = j->info;
				j->info = aux;
			}
			else if (i->info.pret == j->info.pret)
			{
				if (strcmp(i->info.autor, j->info.autor) > 0)
				{
					Carte aux = i->info;
					i->info = j->info;
					j->info = aux;
				}
			}
		}
	}
}
int main()
{
	Nod* cap = citireListaDinFisier("carti.txt");

	afisareLista(cap);

	float medie = pretMediu(cap);
	printf("Pretul mediu al cartilor este: %.2f\n\n", medie);

	Nod* cartiScumpe = copiazaCartiScumpe(cap, 50);
	// afisareLista(cartiScumpe);

	/*printf("Dupa stergere carti categoria A:\n");
	stergeCartiCategorie(&cap, 'A');
	afisareLista(cap); */

	printf("Dupa sortare:\n");
	sortareListaDupaPret(cap);
	afisareLista(cap);

	dezalocare(&cartiScumpe);
	dezalocare(&cap);

	return 0;
}