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

void inserareInArbore(NodArbore** radacina, Telefon telefon)
{
	if (*radacina == NULL)
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
	}
}
NodArbore* citireArboreDinFisier(const char* numeFisier)
{
	NodArbore* radacina = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (file)
	{
		while (!feof(file))
		{
			Telefon tel = citireTelefonDinFisier(file);
			inserareInArbore(&radacina, tel);
		}
		fclose(file);
	}
	return radacina;
}
void afisareTelefoaneDinArboreInordine(NodArbore* radacina)
{
	if (radacina != NULL)
	{
		afisareTelefoaneDinArboreInordine(radacina->stanga);
		afisareTelefon(radacina->info);
		afisareTelefoaneDinArboreInordine(radacina->dreapta);
	}
}

void afisareTelefoaneDinArborePreordine(NodArbore* radacina)
{
	if (radacina != NULL)
	{
		afisareTelefon(radacina->info);
		afisareTelefoaneDinArborePreordine(radacina->stanga);
		afisareTelefoaneDinArborePreordine(radacina->dreapta);
	}
}

void afisareTelefoaneDinArborePostordine(NodArbore* radacina)
{
	if (radacina != NULL)
	{
		afisareTelefoaneDinArborePostordine(radacina->stanga);
		afisareTelefoaneDinArborePostordine(radacina->dreapta);
		afisareTelefon(radacina->info);
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
Telefon getTelefonById(NodArbore* radacina, int id)
{
	if (radacina == NULL)
	{
		Telefon t;
		t.id = -1;
		t.producator = NULL;
		t.model = NULL;
		return t;
	}
	if (radacina->info.id == id)
	{
		Telefon t = radacina->info;
		t.model = (char*)malloc(strlen(radacina->info.model) + 1);
		strcpy(t.model, radacina->info.model);

		t.producator = (char*)malloc(strlen(radacina->info.producator) + 1);
		strcpy(t.producator, radacina->info.producator);

		return t;
	}

	if (radacina->info.id < id)
	{
		return getTelefonById(radacina->dreapta, id);
	}

	if (radacina->info.id > id)
	{
		return getTelefonById(radacina->stanga, id);
	}
}
int determinaNumarNoduri(NodArbore* radacina)
{
	if (radacina != NULL)
	{
		return 1 + determinaNumarNoduri(radacina->stanga)+ determinaNumarNoduri(radacina->dreapta);
	}
	return 0;
}
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
float calculeazaPretTotal(NodArbore* radacina)
{
	if (radacina)
	{
		return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
	}
	return 0;
}
float calculeazaPretTelefoaneProducator(NodArbore* radacina, const char* producator)
{
	if (radacina != NULL)
	{
		float suma = 0;
		suma += calculeazaPretTelefoaneProducator(radacina->stanga, producator);
		suma += calculeazaPretTelefoaneProducator(radacina->dreapta, producator);

		if (strcmp(radacina->info.producator, producator) == 0)
		{
			suma += radacina->info.pret;
		}

		return suma;
	}

	return 0;
}
int main()
{
	NodArbore* radacina = citireArboreDinFisier("telefoane_arbore.txt");
	afisareTelefoaneDinArborePreordine(radacina);

	printf("Nr noduri: %d\n", determinaNumarNoduri(radacina));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
	printf("Pret total: %.2f\n", calculeazaPretTotal(radacina));

	printf("Pret total Apple: %.2f\n",calculeazaPretTelefoaneProducator(radacina, "Apple"));
	dezalocareArbore(&radacina);
	return 0;
}