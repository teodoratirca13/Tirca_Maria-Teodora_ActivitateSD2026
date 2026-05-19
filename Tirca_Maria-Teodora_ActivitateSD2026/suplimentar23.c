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

	c.categorie = (strtok(NULL, sep))[0];
	return c;
}

struct NodArbore
{
	Carte info;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct NodArbore NodArbore;

void adaugaCarteInArbore(NodArbore** radacina, Carte carte)
{
	if (*radacina == NULL)
	{
		NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
		nod->info = carte;
		nod->stanga = NULL;
		nod->dreapta = NULL;

		*radacina = nod;
	}
	else
	{
		if (carte.id > (*radacina)->info.id)
		{
			//adaugam in dreapta
			adaugaCarteInArbore(&(*radacina)->dreapta, carte);
		}
		else
		{
			adaugaCarteInArbore(&(*radacina)->stanga, carte);
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
			Carte carte = citireCarteDinFisier(file);
			adaugaCarteInArbore(&radacina, carte);
		}
		fclose(file);
	}
	return radacina;
}

//afisarile
void afisareArboreInordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareArboreInordine(radacina->stanga);
		afisareCarte(radacina->info);
		afisareArboreInordine(radacina->dreapta);
	}
}
void afisareArborePreordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareCarte(radacina->info);
		afisareArborePreordine(radacina->stanga);
		afisareArborePreordine(radacina->dreapta);
	}
}
void afisareArborePostordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareArborePostordine(radacina->stanga);
		afisareArborePostordine(radacina->dreapta);
		afisareCarte(radacina->info);
	}
}
void dezalocareArbore(NodArbore** radacina)
{
	if (*radacina)
	{
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);

		if ((*radacina)->info.titlu)
		{
			free((*radacina)->info.titlu);
		}
		if ((*radacina)->info.autor)
		{
			free((*radacina)->info.autor);
		}

		free(*radacina);
		*radacina = NULL;
	}
}
Carte getCarteById(NodArbore* radacina, int idCautat)
{
	if (radacina==NULL)
	{
		Carte c;
		c.id = -1;
		c.autor = NULL;
		c.titlu = NULL;
		return c;
	}
	if (radacina->info.id == idCautat)
	{
		Carte c = radacina->info;
		c.titlu = (char*)malloc(strlen(radacina->info.titlu) + 1);
		strcpy(c.titlu, radacina->info.titlu);
		c.autor = (char*)malloc(strlen(radacina->info.autor) + 1);
		strcpy(c.autor, radacina->info.autor);

		return c;
	}
	else
	{
		if (idCautat > radacina->info.id)
		{
			return getCarteById(radacina->dreapta, idCautat);
		}
		else
		{
			return getCarteById(radacina->stanga, idCautat);
		}
	}
}
int determinaNumarNoduri(NodArbore* radacina)
{
	if (radacina == NULL)
	{
		return 0;
	}
	return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}
int calculeazaInaltimeArbore(NodArbore* radacina)
{
	if (radacina == NULL)
	{
		return 0;
	}
	int inaltimeStanga = calculeazaInaltimeArbore(radacina->stanga);
	int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dreapta);
	return 1 + max(inaltimeStanga, inaltimeDreapta);
}
float calculeazaPretTotal(NodArbore* radacina)
{
	if (radacina == NULL)
	{
		return 0;
	}
	return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
}

float calculeazaPretulPretulCartilorAutor(NodArbore* radacina, const char* autor)
{
	if (radacina == NULL)
	{
		return 0;
	}
	float  suma;
	suma = calculeazaPretulPretulCartilorAutor(radacina->stanga,autor) + calculeazaPretulPretulCartilorAutor(radacina->dreapta,autor);
	if (strcmp(radacina->info.autor, autor) == 0)
	{
		suma += radacina->info.pret;
	}
	return suma;
}
int main()
{
	NodArbore* radacina = citireArboreDinFisier("carti_arbore.txt");
	printf("inordine:\n");
	afisareArboreInordine(radacina);

	printf("preordine:\n");
	afisareArborePreordine(radacina);

	printf("postordine:\n");
	afisareArborePostordine(radacina);

	printf("Numar noduri:%d\n", determinaNumarNoduri(radacina));
	printf("Inaltimea arborelui:%d\n", calculeazaInaltimeArbore(radacina));
	printf("Pretul total al cartilor:%.2f\n", calculeazaPretTotal(radacina));
	printf("Pretul total al cartilor lui Liviu Rebreanu:%.2f\n", calculeazaPretulPretulCartilorAutor(radacina, "Liviu Rebreanu"));

	Carte c = getCarteById(radacina, 3);

	if (c.id != -1)
	{
		printf("\nCarte gasita:\n");
		afisareCarte(c);

		free(c.titlu);
		free(c.autor);
	}
	else
	{
		printf("Cartea nu a fost gasita\n");
	}

	return 0;
}