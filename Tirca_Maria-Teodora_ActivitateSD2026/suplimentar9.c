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
//CHAINING

struct Nod
{
	Carte info;
	struct Nod* next;
};
typedef struct Nod Nod;
struct HashTable
{
	int dim;
	Nod** vector;
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
void afisareListaCarti(Nod* cap)
{
	while (cap != NULL)
	{
		afisareCarte(cap->info);
		cap = cap->next;
	}
}
void adaugaCarteInLista(Nod* cap, Carte carteNoua)
{
	//la final doar daca exista deja
	while (cap->next != NULL)
	{
		cap = cap->next;
	}
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = carteNoua;
	nod->next = NULL;

	cap->next = nod;
}
HashTable initializareTabelaDispersie(int dimensiune)
{
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	//punem NULL;
	for (int i = 0; i < dimensiune; i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}
int calculeazaHash(const char* titlu, int dim)
{
	int suma = 0;
	for (int i = 0; i < strlen(titlu); i++)
	{
		suma += titlu[i];
	}
	return suma % dim;
}
void inserareCarteInTabela(HashTable ht, Carte carte)
{
	int poz = calculeazaHash(carte.titlu, ht.dim);
	if (poz >= 0 && poz < ht.dim)
	{
		if (ht.vector[poz] == NULL)//nu avem coliziune
		{
			ht.vector[poz] = (Nod*)malloc(sizeof(Nod));
			ht.vector[poz]->info = carte;
			ht.vector[poz]->next = NULL;
		}
		else adaugaCarteInLista(ht.vector[poz], carte);
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
			inserareCarteInTabela(ht, carte);
		}
		fclose(file);
	}
	return ht;
}
void afisareTabelaDeCarti(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("\nCartile de pe pozitia %d sunt:\n", i + 1);
			afisareListaCarti(ht.vector[i]);
		}
		else
		{
			printf("\nPe pozitia %d nu exista carti\n", i + 1);
		}
	}
}
void dezaocareLista(Nod** cap)
{
	while (*cap)
	{
		Nod* p = *cap;
		*cap = (*cap)->next;
		if (p->info.autor != NULL)
		{
			free(p->info.autor);
		}
		if (p->info.titlu != NULL)
		{
			free(p->info.titlu);
		}
		free(p);
	}
}
void dezalocareTabelaDeDispersie(HashTable* ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		dezaocareLista(&ht->vector[i]);
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float medieLista(Nod* cap)
{
	float suma = 0;
	int contor = 0;
	while (cap != NULL)
	{
		suma += cap->info.pret;
		contor++;
		cap = cap->next;
	}
	if (contor > 0)
		return suma / contor;
	return 0;
}
//returnam doar vectorul de medii
float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {

	float* preturi = NULL;
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
			(*nrClustere)++;
	}
	preturi = (float*)malloc(sizeof(float) * (*nrClustere));
	int j = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			preturi[j] = medieLista(ht.vector[i]);
			j++;
		}
	}
	return preturi;
}
//matrice : pe prima liie pozitia clusterului pe a doua linie pretul mediu
float** calculeazaPozitiiSiPreturiMedii(HashTable ht, int* nrClustere)
{
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
			(*nrClustere)++;
	}

	//alocam martrice
	float** preturi = (float**)malloc(sizeof(float*) * 2);
	for (int i = 0; i < 2; i++)
	{
		preturi[i] = (float*)malloc(sizeof(float) * (*nrClustere));
	}

	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			float medie = medieLista(ht.vector[i]);
			preturi[0][k] = i;
			preturi[1][k] = medie;
			k++;
		}
	}
	return preturi;
}
void afisareMatrice(float** preturi, int nrClustere)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < nrClustere; j++)
		{
			printf("%.2f  ", preturi[i][j]);
		}
		printf("\n");
	}
}
void dezalocareMatrice(float*** matrice)
{
	//dezalocam liniile
	for (int i = 0; i < 2; i++)
	{
		free((*matrice)[i]);
	}
	free(*matrice); //dezalocam vectorul de pointeri;
	*matrice = NULL;
}

Carte getCarteDinLista(Nod* cap, const char* titlu)
{
	Carte c;
	c.id = -1;
	while (cap && strcmp(cap->info.titlu, titlu) != 0)
	{
		cap = cap->next;
	}
	if (cap)//am gasit
	{
		c = cap->info;
		//alocam campurile pt deep copy
		c.titlu = (char*)malloc(strlen(cap->info.titlu) + 1);
		strcpy(c.titlu, cap->info.titlu);
		c.autor = (char*)malloc(strlen(cap->info.autor) + 1);
		strcpy(c.autor, cap->info.autor);
	}
	return c;
}
Carte getCarteDupaTitlu(HashTable ht, const char* titluCautat)
{
	Carte c;
	c.id = -1;
	int poz = calculeazaHash(titluCautat, ht.dim);
	if (poz >= 0 && poz < ht.dim)
	{
		//acum suntem in cluster ul potrivit
		c = getCarteDinLista(ht.vector[poz], titluCautat);
	}
	return c;
}
void stergereCarteDinLista(Nod** cap, const char* titlu)
{
	if (*cap == NULL)
		return;

	//daca e primul nod
	if (strcmp((*cap)->info.titlu, titlu) == 0)
	{
		Nod* p = *cap;
		*cap = p->next;
		if (p->info.autor != NULL)
			free(p->info.autor);
		if (p->info.titlu != NULL)
			free(p->info.titlu);
		free(p);

		return;
	}
	Nod* p = *cap;
	while (p->next != NULL && strcmp(p->next->info.titlu, titlu) != 0)
	{
		p = p->next;
	}
	//suntem inainte de nodul care trb sters
	if (p->next)
	{
		Nod* temp = p->next;
		p->next = temp->next;

		free(temp->info.titlu);
		free(temp->info.autor);
		free(temp);
	}
}
void stergereCarteDinTabela(HashTable ht, const char* titlu)
{
	int poz = calculeazaHash(titlu, ht.dim);
	if (poz >= 0 && poz < ht.dim)
	{
		stergereCarteDinLista(&ht.vector[poz], titlu);
	}
}
int main()
{
	HashTable ht = citireCartiDinFisier("carti.txt", 3);
	afisareTabelaDeCarti(ht);

	int nrClustere = 0;
	float* preturi = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	printf("Preturile medii per clustere sub forma de vector:\n");
	for (int i = 0; i < nrClustere; i++)
	{
		printf("%.2f  ", preturi[i]);
	}
	free(preturi);	//dezalocam vectorul
	printf("\n\n");

	float** matrice = calculeazaPozitiiSiPreturiMedii(ht, &nrClustere);
	afisareMatrice(matrice, nrClustere);
	dezalocareMatrice(&matrice);

	printf("\n");

	Carte c = getCarteDupaTitlu(ht, "Crima si pedeapsa");
	if (c.id != -1)
	{
		printf("\nCartea cautata:\n");
		afisareCarte(c);

		//dezalocam aceasta carte
		if (c.autor != NULL)
		{
			free(c.autor);
		}
		if (c.titlu != NULL)
		{
			free(c.titlu);
		}
	}
	else
	{
		printf("\nNu exista aceasta carte");
	}

	stergereCarteDinTabela(ht, "Crima si pedeapsa");
	printf("\nDupa stergerea cartii 'Crima si pedeapsa':\n");
	afisareTabelaDeCarti(ht);

	dezalocareTabelaDeDispersie(&ht);
	return 0;
}