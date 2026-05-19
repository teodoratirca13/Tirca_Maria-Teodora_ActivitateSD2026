#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct StructuraAngajat
{
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

	a.departament = (strtok(NULL, sep))[0];

	return a;
}

struct NodArbore {
	Angajat info;
	struct NodArbore* dreapta;
	struct NodArbore* stanga;
};
typedef struct NodArbore NodArbore;


int calculeazaInaltimeArbore(NodArbore* radacina)
{
	if (radacina)
	{
		int inaltimeStanga = calculeazaInaltimeArbore(radacina->stanga);
		int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dreapta);

		if (inaltimeStanga > inaltimeDreapta)
		{
			return 1 + inaltimeStanga;
		}
		return 1 + inaltimeDreapta;
	}
	return 0;
}

int verificareEchilibru(NodArbore* radacina)
{
	int st = calculeazaInaltimeArbore(radacina->stanga);
	int dr = calculeazaInaltimeArbore(radacina->dreapta);

	return st - dr;
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
void inserareInArboreEchilibrat(NodArbore** radacina,Angajat angajat)
{
	if (*radacina == NULL)
	{
		NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
		nod->info = angajat;
		nod->stanga = NULL;
		nod->dreapta = NULL;
		*radacina = nod;
	}
	else
	{
		if (angajat.id > (*radacina)->info.id)
		{
			inserareInArboreEchilibrat(&((*radacina)->dreapta), angajat);
		}
		else
		{
			inserareInArboreEchilibrat(&((*radacina)->stanga), angajat);
		}

		int gradEchilibru = verificareEchilibru(*radacina);
		if (gradEchilibru == 2) //dezechilibru pe stanga
		{
			if (verificareEchilibru((*radacina)->stanga) == -1)
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		if (gradEchilibru == -2)  //dezechilibru pe  dreapta
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
			Angajat angajat = citireAngajatDinFisier(file);
			inserareInArboreEchilibrat(&radacina, angajat);
		}
		fclose(file);
	}
	return radacina;
}
void afisareArborePreordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareAngajat(radacina->info);
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

		if ((*radacina)->info.functie)
		{
			free((*radacina)->info.functie);
		}
		if ((*radacina)->info.nume)
		{
			free((*radacina)->info.nume);
		}
		free(*radacina);
		*radacina = NULL;
	}
}
Angajat getAngajatById(NodArbore* radacina, int idCautat)
{
	if (radacina == NULL)
	{
		Angajat a;
		a.id = -1;
		a.functie = NULL;
		a.nume = NULL;
		return a;
	}

	if (radacina->info.id == idCautat)
	{
		Angajat a = radacina->info;
		a.nume = (char*)malloc(strlen(radacina->info.nume) + 1);
		strcpy(a.nume, radacina->info.nume);
		a.functie = (char*)malloc(strlen(radacina->info.functie) + 1);
		strcpy(a.functie, radacina->info.functie);
		return a;
	}
	else
	{
		if (idCautat > radacina->info.id)
		{
			return getAngajatById(radacina->dreapta, idCautat);
		}
		else
		{
			return getAngajatById(radacina->stanga, idCautat);
		}
	}
}
int calculeazaNumarNoduri(NodArbore* radacina)
{
	if (radacina == NULL)
	{
		return 0;
	}
	return 1 + calculeazaNumarNoduri(radacina->stanga) + calculeazaNumarNoduri(radacina->dreapta);
}

float calculeazaSalariulTotal(NodArbore* radacina)
{
	if (radacina == NULL)
	{
		return 0;
	}
	return radacina->info.salariu + calculeazaSalariulTotal(radacina->stanga) + calculeazaSalariulTotal(radacina->dreapta);
}
float calculeazaSalariulAngajatilorDinDepartament(NodArbore* radacina, unsigned char departament)
{
	if (radacina == NULL)
	{
		return 0;
	}
	float total=0;
	total += calculeazaSalariulAngajatilorDinDepartament(radacina->stanga,departament);
	total += calculeazaSalariulAngajatilorDinDepartament(radacina->dreapta,departament);

	if (radacina->info.departament == departament)
	{
		total += radacina->info.salariu;
	}
	return total;
}
int main()
{
	NodArbore* radacina = citireArboreDinFisier("angajati.txt");
	afisareArborePreordine(radacina);

	printf("Numar noduri in arbore: %d\n", calculeazaNumarNoduri(radacina));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
	printf("Salariu total angajati: %.2f\n", calculeazaSalariulTotal(radacina));
	printf("Salariu total departament I: %.2f\n",calculeazaSalariulAngajatilorDinDepartament(radacina, 'I'));


	Angajat rezultat = getAngajatById(radacina, 3);

	if (rezultat.id != -1)
	{
		printf("Angajatul cautat este:\n");
		afisareAngajat(rezultat);

		free(rezultat.nume);
		free(rezultat.functie);
	}
	else
	{
		printf("Id cautat nu exista!\n");
	}
	dezalocareArbore(&radacina);
	return 0;
}