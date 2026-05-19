#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct StructuraAngajat {
	int id;
	int varsta;
	float salariu;
	char* nume;
	char* functie;
	unsigned char departament;
};
typedef struct StructuraAngajat Angajat;

void afisareAngajat(Angajat angajat)
{
	printf("Id:%d\n", angajat.id);
	printf("Varsta:%d\n", angajat.varsta);
	printf("Salariu:%.2f\n", angajat.salariu);
	printf("Nume:%s\n", angajat.nume);
	printf("Functie:%s\n", angajat.functie);
	printf("Departament:%c\n", angajat.departament);
	printf("\n");
}

Angajat citireAngajatDinFisier(FILE* file)
{
	char linie[200];
	fgets(linie, 200, file);
	char sep[3] = ",\n";

	Angajat a;

	a.id = atoi(strtok(linie, sep));
	a.varsta = atoi(strtok(NULL, sep));
	a.salariu = atof(strtok(NULL, sep));

	char* aux;

	aux = strtok(NULL, sep);
	a.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(a.nume, aux);

	aux = strtok(NULL, sep);
	a.functie = (char*)malloc(strlen(aux) + 1);
	strcpy(a.functie, aux);

	a.departament = strtok(NULL, sep)[0];

	return a;
}

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodPrincipal
{
	Angajat info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

struct NodSecundar
{
	NodPrincipal* nodInfo;
	NodSecundar* next;
};

//inserarile
void inserareListaPrincipala(NodPrincipal** cap, Angajat angajat) //inserare la final;
{
	NodPrincipal* nod = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nod->info = angajat;
	nod->next = NULL;
	nod->vecini = NULL;
	
	if (*cap != NULL)
	{
		NodPrincipal* p = *cap;
		while (p->next)
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
void inserareListaSecundara(NodSecundar** cap, NodPrincipal* nodInfo) //inserare la inceput
{
	NodSecundar* nod = (NodSecundar*)malloc(sizeof(NodSecundar));
	nod->nodInfo = nodInfo;
	nod->next = *cap;
	*cap = nod;
}

NodPrincipal* cautaNodDupaId(NodPrincipal* cap, int idCautat)
{
	while (cap && cap->info.id != idCautat)
	{
		cap = cap->next;
	}
	return cap;
}
void inserareMuchie(NodPrincipal* graf, int idStart, int idStop)
{
	NodPrincipal* nodStart = cautaNodDupaId(graf, idStart);
	NodPrincipal* nodStop = cautaNodDupaId(graf, idStop);

	if (nodStart && nodStop)
	{
		inserareListaSecundara(&nodStart->vecini, nodStop);
		inserareListaSecundara(&nodStop->vecini, nodStart);
	}
}

NodPrincipal* citireNoduriDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	NodPrincipal* graf = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Angajat angajat = citireAngajatDinFisier(file);
			inserareListaPrincipala(&graf, angajat);
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

void afisareListaSecundara(NodPrincipal* graf,int id)
{
	NodPrincipal* nod = cautaNodDupaId(graf, id);
	if (nod)
	{
		NodSecundar* p = nod->vecini;
		while (p)
		{
			afisareAngajat(p->nodInfo->info);
			p = p->next;
		}
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
void dezalocareGraf(NodPrincipal** graf)
{
	while (*graf)
	{
		NodPrincipal* nod = *graf;
		*graf = (*graf)->next;

		if (nod->info.functie)
		{
			free(nod->info.functie);
		}
		if (nod->info.nume)
		{
			free(nod->info.nume);
		}
		dezalocareListaSecundara(&nod->vecini);

		free(nod);
	}
}
int main()
{

	NodPrincipal* graf = citireNoduriDinFisier("angajati.txt");

	citireMuchiiDinFisier("muchii_angajati.txt", graf);

	afisareListaSecundara(graf, 10);
	dezalocareGraf(&graf);

	return 0;
}