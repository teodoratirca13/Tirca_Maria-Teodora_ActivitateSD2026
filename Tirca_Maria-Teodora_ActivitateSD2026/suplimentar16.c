#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct StructuraMasina
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod
{
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;
void afisareMasina(Masina masina)
{
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}
Masina citireMasinaDinFisier(FILE* file)
{
	Masina m;
	char linie[100];
	char separator[3] = ",\n";
	fgets(linie, 100, file);

	m.id = atoi(strtok(linie, separator));
	m.nrUsi = atoi(strtok(NULL, separator));
	m.pret = atof(strtok(NULL, separator));

	char* aux1 = strtok(NULL, separator);
	m.model = (char*)malloc(sizeof(char) * (strlen(aux1) + 1));
	strcpy(m.model, aux1);
	char* aux2 = strtok(NULL, separator);
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux2) + 1));
	strcpy(m.numeSofer, aux2);

	m.serie = (strtok(NULL, separator))[0];
	return m;
}
void adaugaLaInceput(Nod** cap, Masina masina)
{
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = masina;

	if (*cap == NULL)
	{
		*cap = nod;
		nod->next = *cap;
	}
	else {
		Nod* p = *cap;
		while (p->next != *cap)
		{
			p = p->next;
		}
		nod->next = *cap;
		*cap = nod;
		p->next = *cap;
	}
}
void adaugaLaFinal(Nod** cap, Masina masina)
{
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = masina;
	if (*cap == NULL)
	{
		*cap = nod;
		nod->next = *cap;
	}
	else
	{
		Nod* p = *cap;
		while (p->next != *cap)
		{
			p = p->next;
		}
		p->next = nod;
		nod->next = *cap;
	}
}
void dezalocareLista(Nod** cap)
{
	if (*cap == NULL)
		return;

	//rupem legatura circulara
	Nod* p = *cap;
	while (p->next != *cap)
	{
		p = p->next;
	}
	p->next = NULL;//ruperea

	//acum avem lista simpla
	while (*cap != NULL)
	{
		Nod* aux = *cap;
		*cap = aux->next;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
}
void afisareLista(Nod* cap)
{
	Nod* p = cap;
	if (cap != NULL)
	{
		while (p->next != cap)
		{
			afisareMasina(p->info);
			p = p->next;
		}
		afisareMasina(p->info);
	}
}
Nod* citireListaDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Nod* lista = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Masina masina = citireMasinaDinFisier(file);
			adaugaLaFinal(&lista, masina);
		}
		fclose(file);
	}
	return lista;
}
int main()
{
	Nod* lista = citireListaDinFisier("masini.txt");
	afisareLista(lista);
	dezalocareLista(&lista);
	return 0;
}