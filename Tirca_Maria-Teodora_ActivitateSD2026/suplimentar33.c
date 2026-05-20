#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct StructuraTelefon
{
	int id;
	int memorie;
	float pret;
	char* model;
	char* producator;
	unsigned char categorie;
};
typedef struct StructuraTelefon Telefon;

void afisareTelefon(Telefon telefon)
{
	printf("Id: %d\n", telefon.id);
	printf("Memorie: %d GB\n", telefon.memorie);
	printf("Pret: %.2f\n", telefon.pret);
	printf("Model: %s\n", telefon.model);
	printf("Producator: %s\n", telefon.producator);
	printf("Categorie: %c\n", telefon.categorie);
	printf("\n");
}

Telefon citireTelefonDinFisier(FILE* file)
{
	Telefon t;
	char linie[100];
	char separator[3] = ",\n";

	fgets(linie, 100, file);

	t.id = atoi(strtok(linie, separator));
	t.memorie = atoi(strtok(NULL, separator));
	t.pret = atof(strtok(NULL, separator));

	char* aux1 = strtok(NULL, separator);
	t.model = (char*)malloc(strlen(aux1) + 1);
	strcpy(t.model, aux1);

	char* aux2 = strtok(NULL, separator);
	t.producator = (char*)malloc(strlen(aux2) + 1);
	strcpy(t.producator, aux2);

	t.categorie = strtok(NULL, separator)[0];

	return t;
}

typedef struct NodArbore NodArbore;
struct NodArbore
{
	Telefon info;
	NodArbore* stanga;
	NodArbore* dreapta;
};

int calculeazaInaltimeArbore(NodArbore* radacina)
{
	if (radacina)
	{
		int st = calculeazaInaltimeArbore(radacina->stanga);
		int dr = calculeazaInaltimeArbore(radacina->dreapta);

		return 1 + fmax(st, dr);
	}
	return 0;
}
int verificareEchilibru(NodArbore* radacina)
{
	if (radacina)
	{
		int st = calculeazaInaltimeArbore(radacina->stanga);
		int dr = calculeazaInaltimeArbore(radacina->dreapta);
		return st - dr;
	}
	return 0;
}
void rotireStanga(NodArbore** radacina)
{
	NodArbore* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = *radacina;
	*radacina = aux;
}
void rotireDreapta(NodArbore** radacina)
{
	NodArbore* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = *radacina;
	*radacina = aux;
}
void inserareInArbore(NodArbore** radacina, Telefon telefon)
{
	if (*radacina==NULL)
	{
		NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
		nod->info = telefon;
		nod->stanga = NULL;
		nod->dreapta = NULL;

		*radacina = nod;
	}
	else
	{
		if (telefon.id > (*radacina)->info.id)
		{
			inserareInArbore(&(*radacina)->dreapta, telefon);
		}
		else
		{
			inserareInArbore(&(*radacina)->stanga, telefon);
		}

		int gradDeEchilibru = verificareEchilibru(*radacina);
		if (gradDeEchilibru == 2) // stanga
		{
			if (verificareEchilibru((*radacina)->stanga) == -1)
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		if (gradDeEchilibru == -2)
		{
			if (verificareEchilibru((*radacina)->dreapta) == 1)
			{
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotireStanga(radacina);
		}
	}
}
NodArbore* citireArboreDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	NodArbore* radacina = NULL;

	if (file)
	{
		while (!feof(file))
		{
			Telefon t = citireTelefonDinFisier(file);
			inserareInArbore(&radacina, t);
		}

		fclose(file);
	}

	return radacina;
}

void afisareArborePreordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareTelefon(radacina->info);
		afisareArborePreordine(radacina->stanga);
		afisareArborePreordine(radacina->dreapta);
	}
}

void dezalocareArbore(NodArbore** radacina)
{

	if (*radacina)
	{
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);

		if ((*radacina)->info.model)
		{
			free((*radacina)->info.model);
		}

		if ((*radacina)->info.producator)
		{
			free((*radacina)->info.producator);
		}

		free(*radacina);
		*radacina = NULL;
	}
}
int main()
{
	NodArbore* radacina = citireArboreDinFisier("telefoane.txt");

	printf("Afisare preordine:\n");
	afisareArborePreordine(radacina);

	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));

	dezalocareArbore(&radacina);
	return 0;
}