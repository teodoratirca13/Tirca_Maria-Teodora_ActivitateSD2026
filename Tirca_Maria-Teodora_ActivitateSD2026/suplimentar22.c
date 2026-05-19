#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
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

void afisareCarte(Carte carte)
{
	printf("Id:%d\n", carte.id);
	printf("Nr pagini:%d\n", carte.nrPagini);
	printf("Pret:%.2f\n", carte.pret);
	printf("Titlu:%s\n", carte.titlu);
	printf("Autor:%s\n", carte.autor);
	printf("Categorie:%c\n", carte.categorie);
	printf("\n");
}
Carte citireCarteDinFisier(FILE* file)
{
	char linie[200];
	fgets(linie, 200, file);
	char sep[3] = ",\n";

	Carte c;
	c.id = atoi(strtok(linie, sep));
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));

	char* aux;
	aux = strtok(NULL, sep);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);

	aux = strtok(NULL, sep);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);

	c.categorie = strtok(NULL, sep)[0];
	return c;
}


typedef struct NodSecundar NodSecundar;
typedef struct NodPrincipal NodPrincipal;

struct NodPrincipal
{
	Carte info;
	NodSecundar* vecini;
	struct NodPrincipal* next;
};

struct NodSecundar
{
	NodPrincipal* nodInfo;
	NodSecundar* next;
};

//inserare la sfarsit
void inserareListaPrincipala(NodPrincipal** cap, Carte carteNoua)
{
	NodPrincipal* nodNou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nodNou->info = carteNoua;
	nodNou->next = NULL;
	nodNou->vecini = NULL;

	if (*cap != NULL)
	{
		NodPrincipal* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = nodNou;
	}
	else
	{
		*cap = nodNou;
	}
}

//inserare la inceput
void inserareListaSecundara(NodSecundar** cap, NodPrincipal* nodInfo)  //adresa unui nod principal
{
	NodSecundar* nodNou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nodNou->nodInfo = nodInfo;
	nodNou->next = *cap;
	*cap = nodNou;
}

NodPrincipal* cautaNodDupaID(NodPrincipal* cap, int id)
{
	while (cap && cap->info.id!=id)
	{
		cap = cap->next;
	}
	return cap;
}
void inserareMuchie(NodPrincipal* graf, int idStart, int idStop)
{
	NodPrincipal* nodStart = cautaNodDupaID(graf, idStart);
	NodPrincipal* nodStop = cautaNodDupaID(graf, idStop);

	if (nodStart != NULL && nodStop != NULL)
	{
		inserareListaSecundara(&(nodStart->vecini), nodStop);
		inserareListaSecundara(&(nodStop->vecini), nodStart);

	}
}
NodPrincipal* citireNoduriCartiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	NodPrincipal* graf = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			inserareListaPrincipala(&graf, carte);
		}
		fclose(file);
	}
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier,NodPrincipal*graf)
{
	FILE* file = fopen(numeFisier, "r");
	if (file)
	{
		while (!feof(file))
		{
			int idStart, idStop;
			fscanf(file, "%d %d", &idStart, &idStop);
			inserareMuchie(graf, idStart, idStop);
		}
		fclose(file);
	}
}
void dezalocareListaSecundara(NodSecundar** cap)
{
	while (*cap)
	{
		NodSecundar* p = *cap;
		*cap = (*cap)->next;
		
		free(p);
	}
}

void dezalocareNoduriGraf(NodPrincipal** graf)
{
	while (*graf)
	{
		NodPrincipal* p = *graf;
		*graf = (*graf)->next;

		if (p->info.autor != NULL)
		{
			free(p->info.autor);
		}
		if (p->info.titlu != NULL)
		{
			free(p->info.titlu);
		}
		dezalocareListaSecundara(&(p->vecini));

		free(p);
	}
}
void afisareListaSecundara(NodPrincipal* graf, int id)
{
	NodPrincipal* nod = cautaNodDupaID(graf, id);
	NodSecundar* cap = nod->vecini;
	while (cap) {
		afisareCarte(cap->nodInfo->info);
		cap = cap->next;
	}
}
int main()
{
	NodPrincipal* graf = citireNoduriCartiDinFisier("carti.txt");
	citireMuchiiDinFisier("muchii_carti.txt", graf);
	afisareListaSecundara(graf, 1);

	dezalocareNoduriGraf(&graf);
	return 0;
}
