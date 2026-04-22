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
	struct Nod* prev;
};
typedef struct Nod Nod;
struct ListaDubla
{
	Nod* start;
	Nod* end;
};
typedef struct ListaDubla ListaDubla;

void afisareCarte(Carte carte)
{
	printf("Id: %d\n", carte.id);
	printf("Nr. pagini : %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Categorie: %c\n\n", carte.categorie);
}
void afisareListaDeLaInceput(ListaDubla lista)
{
	Nod* p = lista.start;
	while (p != NULL)
	{
		afisareCarte(p->info);
		p = p->next;
	}
}
void afisareListaDeLaSfarsit(ListaDubla lista)
{
	Nod* p = lista.end;
	while (p != NULL)
	{
		afisareCarte(p->info);
		p = p->prev;
	}
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

	char* aux1 = strtok(NULL, separator);
	c.titlu = (char*)malloc(sizeof(char) * (strlen(aux1) + 1));
	strcpy(c.titlu, aux1);
	char* aux2 = strtok(NULL, separator);
	c.autor = (char*)malloc(sizeof(char) * (strlen(aux2) + 1));
	strcpy(c.autor, aux2);

	c.categorie = (strtok(NULL, separator))[0];
	return c;
}
void adaugaLaFinal(ListaDubla* lista, Carte carteNoua)
{
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = carteNoua;
	nod->next = NULL;
	nod->prev = lista->end;
	if (lista->start == NULL)//lista e goala
	{
		lista->start = nod;
	}
	else
	{
		lista->end->next = nod;
	}
	lista->end = nod;
}
void adaugaLaInceput(ListaDubla* lista, Carte carteNoua)
{

	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = carteNoua;
	nod->prev = NULL;
	nod->next = lista->start;

	if (lista->start == NULL)
	{
		lista->end = nod;
	}
	else
	{
		lista->start->prev = nod;
	}
	lista->start = nod;
}
void adaugaPePozitie(ListaDubla* lista, Carte carte, int poz)
{
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = carte;
	if (lista->start == NULL)
	{
		lista->start = nod;
		lista->end = nod;
		nod->next = NULL;
		nod->prev = NULL;
	}
	else
	{
		if (poz == 1)
		{
			nod->next = lista->start;
			nod->prev = NULL;
			lista->start->prev = nod;
			lista->start = nod;
		}
		else
		{
			Nod* t = lista->start;
			int k = 1;
			while (t->next != NULL && k < poz - 1)
			{
				t = t->next;
				k++;
			}
			if (t->next != NULL)
			{
				//nu e ultimul
				Nod* p = t->next;

				nod->next = p;
				nod->prev = t;
				t->next = nod;
				p->prev = nod;
			}
			else
			{
				//este ultimul;
				nod->next = NULL;
				nod->prev = lista->end;
				lista->end->next = nod;
				lista->end = nod;
			}
		}
	}
}
ListaDubla citireListaDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.end = NULL;
	lista.start = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			adaugaLaFinal(&lista, carte);
			//adaugaLaInceput(&lista, carte);
		}
		fclose(file);
	}
	return lista;
}
void dezalocare(ListaDubla* lista)
{
	Nod* p = lista->start;
	while (p != NULL)
	{
		Nod* aux = p;
		p = p->next;

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
	lista->end = NULL;
	lista->start = NULL;
}
float pretMediu(ListaDubla lista)
{
	Nod* p = lista.start;
	float suma = 0;
	int contor = 0;
	while (p)
	{
		suma += p->info.pret;
		contor++;
		p = p->next;
	}
	if (contor > 0)
	{
		return suma / contor;
	}
	return 0;
}
void stergeCarteDupaId(ListaDubla* lista, int id)
{
	if (lista->start == NULL)
	{
		return;
	}
	Nod* p = lista->start;
	while (p != NULL && p->info.id != id)
	{
		p = p->next;
	}
	if (p == NULL) //nu exista
		return;


	if (p->prev == NULL)//e primul
	{
		lista->start = p->next;
		if (lista->start != NULL)
		{
			lista->start->prev = NULL;
		}
	}
	else
	{
		p->prev->next = p->next;
	}

	if (p->next != NULL)//nu e ultimul
	{
		p->next->prev = p->prev;
	}
	else //este ultimu
	{
		lista->end = p->prev;
	}

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
char* getTitluCarteScumpa(ListaDubla lista)
{
	if (lista.start != NULL)
	{
		Nod* p = lista.start;
		Nod* Max = lista.start;
		while (p)
		{
			if (Max->info.pret < p->info.pret)
			{
				Max = p;
			}
			p = p->next;
		}
		char* rezultat = (char*)malloc(strlen(Max->info.titlu) + 1);
		strcpy(rezultat, Max->info.titlu);
		return rezultat;
	}
	return NULL;
}
int main()
{
	ListaDubla lista = citireListaDinFisier("carti.txt");
	//afisareListaDeLaInceput(lista);
	//afisareListaDeLaSfarsit(lista);

	float medie = pretMediu(lista);
	printf("Pretul mediu al unei carti: %.2f\n", medie);

	//stergeCarteDupaId(&lista, 3);
	//afisareListaDeLaInceput(lista);

	char* titlu = getTitluCarteScumpa(lista);

	if (titlu != NULL)
	{
		printf("Titlul cartii celei mai scumpe este: %s\n", titlu);
		free(titlu);
	}

	dezalocare(&lista);
	return 0;
}