#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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

struct NodArbore {
	Apartament info;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct NodArbore NodArbore;

int calculeazaInaltimeArbore(NodArbore* radacina)
{
	if (radacina != NULL)
	{
		int inaltimeSt = calculeazaInaltimeArbore(radacina->stanga);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dreapta);

		return 1 + fmax(inaltimeSt, inaltimeDr);
	}
	return 0;
}
int verificareEchilibru(NodArbore* radacina)
{
	if (radacina)
	{
		int stanga = calculeazaInaltimeArbore(radacina->stanga);
		int dreapta= calculeazaInaltimeArbore(radacina->dreapta);

		return stanga - dreapta;
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
void inserareInAbore(NodArbore** radacina, Apartament apartament)
{
	if (*radacina == NULL)
	{
		NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
		nod->info = apartament;
		nod->stanga = NULL;
		nod->dreapta = NULL;
		*radacina = nod;
	}
	else
	{
		if (apartament.id > (*radacina)->info.id)
		{
			inserareInAbore(&(*radacina)->dreapta, apartament);
		}
		else
		{
			inserareInAbore(&(*radacina)->stanga, apartament);
		}

		int gradEchilibru = verificareEchilibru(*radacina);
		if (gradEchilibru == 2) //dezechilibru stanga
		{
			if (verificareEchilibru((*radacina)->stanga)== -1)
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		if (gradEchilibru == -2) //dezechilibru dreapta
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
			Apartament a = citireApartamentDinFisier(file);
			inserareInAbore(&radacina, a);
		}
		fclose(file);
	}
	return radacina;
}
void afisareArborePreordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareApartament(radacina->info);
		afisareArborePreordine(radacina->stanga);
		afisareArborePreordine(radacina->dreapta);
	}
}

void afisareArboreInordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareArboreInordine(radacina->stanga);
		afisareApartament(radacina->info);
		afisareArboreInordine(radacina->dreapta);
	}
}

void afisareArborePostordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareArborePostordine(radacina->stanga);
		afisareArborePostordine(radacina->dreapta);
		afisareApartament(radacina->info);
	}
}
void dezalocareArbore(NodArbore** radacina)
{
	if (*radacina)
	{
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);

		if ((*radacina)->info.zona)
		{
			free((*radacina)->info.zona);
		}
		if ((*radacina)->info.adresa)
		{
			free((*radacina)->info.adresa);
		}
		free(*radacina);
		*radacina = NULL;
	}
}
Apartament getApartamentById(NodArbore* radacina, int idCautat)
{
	if (radacina==NULL)
	{
		Apartament a;
		a.id = -1;
		a.zona = NULL;
	    a.adresa = NULL;
		return a;
	}
	if (radacina->info.id == idCautat)
	{
		Apartament a = radacina->info;
		a.zona = (char*)malloc(strlen(radacina->info.zona) + 1);
		strcpy(a.zona,radacina->info.zona);
		a.adresa = (char*)malloc(strlen(radacina->info.adresa) + 1);
		strcpy(a.adresa, radacina->info.adresa);
		return a;
	}
	else
	{
		if (idCautat > radacina->info.id)
		{
			return getApartamentById(radacina->dreapta, idCautat);
		}
		else
		{
			return getApartamentById(radacina->stanga, idCautat);
		}
	}
}
int calculeazaNumarNoduri(NodArbore* radacina)
{
	if (radacina)
	{
		return 1 + calculeazaNumarNoduri(radacina->stanga) + calculeazaNumarNoduri(radacina->dreapta);
	}
	return 0;
}
float calculeazaPretTotal(NodArbore* radacina)
{
	if (radacina)
	{
		return calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta) + radacina->info.pret;
	}
	return 0;
}
float calculeazaPretApartamenteDinZona(NodArbore* radacina, const char* zona)
{
	if (radacina)
	{
		float suma = 0;
		suma += calculeazaPretApartamenteDinZona(radacina->stanga, zona);
		suma += calculeazaPretApartamenteDinZona(radacina->dreapta, zona);

		if (strcmp(radacina->info.zona, zona) == 0)
		{
			suma += radacina->info.pret;
		}
		return suma;
	}
	return 0;
}
int calculeazaNumarApartamenteCuConfort(NodArbore* radacina, unsigned char confort)
{
	if (radacina)
	{
		int nr = 0;
		nr += calculeazaNumarApartamenteCuConfort(radacina->stanga, confort);
		nr += calculeazaNumarApartamenteCuConfort(radacina->dreapta, confort);
		if (radacina->info.confort == confort)
		{
			nr++;
		}
		return nr;
	}
	return 0;
}
int main()
{
	NodArbore* radacina = citireArboreDinFisier("apartamente.txt");
	afisareArborePreordine(radacina);

	Apartament a = getApartamentById(radacina, 4);
	if (a.zona != NULL)
	{
		printf("Apartamentul cautat:\n");
		afisareApartament(a);

		free(a.zona);
		free(a.adresa);
	}
	else printf("Nu exista apratemtn cu acest id!\n");

	printf("Numarul de noduri:%d\n", calculeazaNumarNoduri(radacina));
	printf("Pretul total al apartamentelor:%.2f\n", calculeazaPretTotal(radacina));
	printf("Pretul total al apartamentelor din zona Militari:%.2f\n", calculeazaPretApartamenteDinZona(radacina, "Militari"));
	printf("Numar apartamente confort A: %d\n", calculeazaNumarApartamenteCuConfort(radacina, 'A'));
	dezalocareArbore(&radacina);
	return 0;
}