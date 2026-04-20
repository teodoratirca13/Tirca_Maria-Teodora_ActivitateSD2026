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

struct HashTable
{
	int dim;
	Carte* vector;
};
typedef struct HashTable HashTable;

Carte citireCarteDinFisier(FILE* file)
{
	char linie[100];
	fgets(linie, 100, file);

	char separator[3] = ",\n";
	Carte c;

	c.id = atoi(strtok(linie, separator));
	c.nrPagini = atoi(strtok(NULL, separator));
	c.pret = atof(strtok(NULL, separator));

	char* aux;

	aux = strtok(NULL, separator);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);

	aux = strtok(NULL, separator);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);

	c.categorie = strtok(NULL, separator)[0];

	return c;
}
void afisareCarte(Carte carte)
{
	printf("Id: %d\n", carte.id);
	printf("Nr. pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Categorie: %c\n\n", carte.categorie);
}
HashTable initializareTabelaDispersie(int dimensiune)
{
	HashTable hash;
	hash.dim = dimensiune;
	hash.vector = (Carte*)malloc(sizeof(Carte) * dimensiune);
	for (int i = 0; i < hash.dim; i++)
	{
		hash.vector[i].titlu = NULL;
		hash.vector[i].autor = NULL;
		hash.vector[i].id = -1;
		hash.vector[i].pret = 0;
		hash.vector[i].nrPagini = 0;
		hash.vector[i].categorie = '-';
	}
	return hash;
}
int calculeazaHash(const char* titlu, int dimensiune)
{
	int suma = 0;
	for (int i = 0; i < strlen(titlu); i++)
	{
		suma += titlu[i];
	}
	return suma % dimensiune;
}
int cautaCarteInTabela(HashTable ht, const char* titlu)
{
	int poz = calculeazaHash(titlu, ht.dim);

	for (int i = poz; i < ht.dim; i++)
	{
		if (ht.vector[i].titlu == NULL)
		{
			return -1;
		}
		else if (strcmp(ht.vector[i].titlu, titlu) == 0)
		{
			return i;
		}
	}

	return -1;
}
void afisareTabelaDeCarti(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i].titlu != NULL)
		{
			printf("\nPozitia %d:\n", i);
			afisareCarte(ht.vector[i]);
		}
		else
		{
			printf("\nPe pozitia %d nu exista carti\n", i);
		}
	}
}
int inserareCarteInTabela(HashTable ht, Carte carte)
{
	int pozitie = calculeazaHash(carte.titlu, ht.dim);

	for (int i = pozitie; i < ht.dim; i++)
	{
		if (ht.vector[i].autor == NULL)
		{
			ht.vector[i] = carte;
			return 1;
		}
	}
	return 0;
}

void redimensionareHashTable(HashTable* ht)
{
	int dimNoua = ht->dim * 2; //marim cu mai mult decat +1
	HashTable htNou = initializareTabelaDispersie(dimNoua);

	for (int i = 0; i < ht->dim; i++)
	{
		if (ht->vector[i].autor != NULL)
		{
			//mutam din tabela veche in cea noua
			inserareCarteInTabela(htNou, ht->vector[i]);
		}
	}

	free(ht->vector);
	ht->vector = htNou.vector;
	ht->dim = htNou.dim;
}
void inserareCarteInTabelaCuRedimensionare(HashTable* ht, Carte carte)
{
	if (inserareCarteInTabela(*ht, carte) == 0) //daca nu reuseste inserarea
	{
		redimensionareHashTable(ht);
		inserareCarteInTabela(*ht, carte);
	}
}
HashTable citireCartiDinFisier(const char* numeFisier, int dim)
{
	FILE* file = fopen(numeFisier, "r");
	HashTable ht = initializareTabelaDispersie(dim);

	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			inserareCarteInTabelaCuRedimensionare(&ht, carte);
		}
		fclose(file);
	}

	return ht;
}
void dezalocareTabelaDeDispersie(HashTable* ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		if (ht->vector[i].titlu != NULL)
		{
			free(ht->vector[i].titlu);
		}
		if (ht->vector[i].autor != NULL)
		{
			free(ht->vector[i].autor);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}
int main()
{
	HashTable ht = citireCartiDinFisier("carti.txt", 4);

	printf("\nTabela initiala:\n");
	afisareTabelaDeCarti(ht);

	dezalocareTabelaDeDispersie(&ht);
	return 0;
}