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

	c.categorie = strtok(NULL, sep)[0];
	return c;
}

typedef struct NodArbore NodArbore;
struct NodArbore
{
	Carte info;
	NodArbore* dreapta;
	NodArbore* stanga;
};

int calculeazaInaltimeArbore(NodArbore* radacina)
{
	if(radacina)
	{
		int dreapta = calculeazaInaltimeArbore(radacina->dreapta);
		int stanga = calculeazaInaltimeArbore(radacina->stanga);
		return 1 + max(stanga, dreapta);
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
int verificareEchilibru(NodArbore* radacina)
{
	int stanga = calculeazaInaltimeArbore(radacina->stanga);
	int dreapta = calculeazaInaltimeArbore(radacina->dreapta);
	return stanga - dreapta;
}

void inserareMasinaInArboreEchilibrat(NodArbore** radacina, Carte carte)
{
	//arbore gol
	if (*radacina == NULL)
	{
		NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
		nod->info = carte;
		nod->stanga = NULL;
		nod->dreapta = NULL;
		*radacina = nod;
	}
	else  //avem radacina
	{
		if (carte.id > (*radacina)->info.id)  //punem in dreapta
		{
			inserareMasinaInArboreEchilibrat(&(*radacina)->dreapta, carte);
		}
		else
		{
			inserareMasinaInArboreEchilibrat(&(*radacina)->stanga, carte);
		}

		//verficare de echilibru
		int gradDeEchilibru = verificareEchilibru(*radacina);
		if (gradDeEchilibru == -2)  //dezechilibru pe partea dreapta
		{
			if (verificareEchilibru((*radacina)->dreapta) == -1)  // dreapta-dreapta
			{
				rotireStanga(radacina);
			}
			if (verificareEchilibru((*radacina)->dreapta) == 1)    //dreapta-stanga
			{
				rotireDreapta(radacina);
				rotireStanga(radacina);
			}
		}
		if (gradDeEchilibru == 2)  //dezechilibru pe partea stanga
		{
			if (verificareEchilibru((*radacina)->stanga) == 1)  //stanga-stanga
			{
				rotireDreapta(radacina);
			}
			if (verificareEchilibru((*radacina)->stanga) == -1)    //stanga-dreapta
			{
				rotireStanga(radacina);
				rotireDreapta(radacina);
			}
		}
	}
}
NodArbore* citireAboreDeCartiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	NodArbore* radacina = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			inserareMasinaInArboreEchilibrat(&radacina, carte);
		}
		fclose(file);
	}
	return radacina;
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
void dezalocareArbore(NodArbore** radacina)
{
	if (*radacina)
	{
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);

		free((*radacina)->info.autor);
		free((*radacina)->info.titlu);

		free(*radacina);
		*radacina = NULL;
	}
}
Carte getCarteById(NodArbore* radacina, int id)
{
	if (radacina == NULL)
	{
		Carte c;
		c.id = -1;
		c.titlu = NULL;
		c.autor = NULL;
		return c;
	}
	//avem radacina
	if (radacina->info.id == id)
	{
		Carte c = radacina->info;
		c.autor = (char*)malloc(strlen(radacina->info.autor) + 1);
		strcpy(c.autor, radacina->info.autor);
		c.titlu = (char*)malloc(strlen(radacina->info.titlu) + 1);
		strcpy(c.titlu, radacina->info.titlu);

		return c;
	}
	else
	{
		if (id > radacina->info.id)
		{
			//cautam in subarbore drept
			return getCarteById(radacina->dreapta, id);
		}
		else
		{
			return getCarteById(radacina->stanga, id);
		}
	}
}
int determinaNumarNoduri(NodArbore* radacina)
{
	if (radacina == NULL)
	{
		return 0;
	}
	return 1 + determinaNumarNoduri(radacina->dreapta) + determinaNumarNoduri(radacina->stanga);
}
float calculeazaPretTotal(NodArbore* radacina)
{
	if (radacina == NULL)
	{
		return 0;
	}
	return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
}
float calculeazaPretulCartilorUnuiAutor(NodArbore* radacina, const char* autor)
{
	if (radacina == NULL)
	{
		return 0;
	}

	float suma = 0;
	if (strcmp(radacina->info.autor, autor) == 0)
	{
		suma += radacina->info.pret;
	}
	suma += calculeazaPretulCartilorUnuiAutor(radacina->stanga, autor);
	suma += calculeazaPretulCartilorUnuiAutor(radacina->dreapta, autor);

}
int main()
{
	NodArbore* radacina = citireAboreDeCartiDinFisier("carti.txt");
	afisareArborePreordine(radacina);

	printf("Numar noduri in arbore:%d\n\n", determinaNumarNoduri(radacina));
	printf("Pretul total al cartilor din arbore:%.2f\n\n", calculeazaPretTotal(radacina));

	Carte rezultat = getCarteById(radacina,3);
	if (rezultat.id != -1)
	{
		printf("Cartea cautata este:\n\n");
		afisareCarte(rezultat);

		free(rezultat.autor);
		free(rezultat.titlu);
	}
	else
	{
		printf("Id cautat nu exista!\n\n");
	}

	printf("Pret carti Liviu Rebreanu:%.2f\n",calculeazaPretulCartilorUnuiAutor(radacina, "Liviu Rebreanu"));
	dezalocareArbore(&radacina);
}