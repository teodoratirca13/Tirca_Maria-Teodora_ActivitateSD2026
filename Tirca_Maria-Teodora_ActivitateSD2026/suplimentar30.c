#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct StructuraApartament {
	int id;
	char* adresa;
	int nrCamere;
	unsigned char confort;
	char* zona;
	float pret;
};
typedef  struct StructuraApartament Apartament;
void afisareApartament(Apartament a)
{
	printf("Id:%d\n", a.id);
	printf("Adresa:%s\n", a.adresa);
	printf("Nr camere:%d\n", a.nrCamere);
	printf("Confort:%c\n", a.confort);
	printf("Zona:%s\n", a.zona);
	printf("Pret:%.2f\n", a.pret);
	printf("\n");
}
Apartament citireApartamentDinFisier(FILE* file)
{
	char linie[200];
	fgets(linie, 200, file);
	char sep[3] = ",\n";
	Apartament a;
	a.id = atoi(strtok(linie, sep));
	a.nrCamere = atoi(strtok(NULL, sep));
	a.pret = atof(strtok(NULL, sep));

	char* aux;
	aux = strtok(NULL, sep);
	a.adresa = (char*)malloc(strlen(aux) + 1);
	strcpy(a.adresa, aux);

	aux = strtok(NULL, sep);
	a.zona = (char*)malloc(strlen(aux) + 1);
	strcpy(a.zona, aux);

	a.confort = (strtok(NULL, sep))[0];
	return a;
}

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodPrincipal
{
	Apartament info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

struct NodSecundar
{
	NodSecundar* next;
	NodPrincipal* nodInfo;
};

//inserarile
void inserareListaPrincipala(NodPrincipal** cap, Apartament a)
{
	//inserare la final
	NodPrincipal* nod = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nod->info = a;
	nod->next = NULL;
	nod->vecini = NULL;
	if (*cap)
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
void inserareListaSecundara(NodSecundar** cap, NodPrincipal* nodInfo)
{
	//inserare la inceput
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
void inserareMuchie(NodPrincipal* cap,int idStart, int idStop)
{
	NodPrincipal* nodStart = cautaNodDupaId(cap,idStart);
	NodPrincipal* nodStop = cautaNodDupaId(cap, idStop);

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
			Apartament a = citireApartamentDinFisier(file);
			inserareListaPrincipala(&graf, a);
		}
		fclose(file);
	}
	return graf;
}
void citireMuchiiDinFisier(const char* numeFisier,NodPrincipal* cap)
{
	FILE* file = fopen(numeFisier, "r");
	if (file)
	{
		while (!feof(file))
		{
			int idStart, idStop;
			fscanf(file, "%d %d", &idStart, &idStop);
			inserareMuchie(cap, idStart, idStop);
		}
		fclose(file);
	}
}
void afisareListaSecundara(NodPrincipal* cap, int id)
{
	NodPrincipal* nod = cautaNodDupaId(cap, id);
	if (nod)
	{
		NodSecundar* p = nod->vecini;
		while (p)
		{
			afisareApartament(p->nodInfo->info);
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
		NodPrincipal* p = *graf;
		*graf = (*graf)->next;
		if (p->info.adresa)
		{
			free(p->info.adresa);
		}
		if (p->info.zona)
		{
			free(p->info.zona);
		}
		dezalocareListaSecundara(&p->vecini);
		free(p);
	}
}
int main()
{
	NodPrincipal* graf = citireNoduriDinFisier("apartamente.txt");
	citireMuchiiDinFisier("muchii_apartamente.txt", graf);
	afisareListaSecundara(graf, 2);
	dezalocareGraf(&graf);
	return 0;
}