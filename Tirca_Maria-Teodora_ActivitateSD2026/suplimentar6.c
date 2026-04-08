#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraStudent
{
	int id;
	char* nume;
	int an;
	unsigned char serie;
	char* specializare;
	float medie;
};
typedef struct StructuraStudent Student;

//cream structua nod
struct Nod
{
	Student info;
	struct Nod* next;
};
typedef struct Nod Nod;


//tabela de dispersie
struct HashTable
{
	int dim;
	//vector de liste
	Nod** vector;
};
typedef struct HashTable HashTable;

Student citireStudentDinFiser(FILE* file)
{
	char buffer[100];
	char separator[3] = ",\n";
	fgets(buffer, 100, file);
	Student s;
	s.id = atoi(strtok(buffer, separator));
	s.an = atoi(strtok(NULL, separator));
	s.medie= atof(strtok(NULL, separator));

	char* temp1 = strtok(NULL, separator);
	s.nume = (char*)malloc(strlen(temp1) + 1);
	strcpy(s.nume, temp1);

	char* temp2 = strtok(NULL, separator);
	s.specializare = (char*)malloc(strlen(temp2) + 1);
	strcpy(s.specializare, temp2);

	s.serie = strtok(NULL, separator)[0];
	return s;
}

void afisareStudent(Student s)
{
	printf("Id:%d\n", s.id);
	printf("Nume:%s\n", s.nume);
	printf("An:%d\n", s.an);
	printf("Serie:%c\n", s.serie);
	printf("Specializare:%s\n", s.specializare);
	printf("Medie:%5.2f\n", s.medie);
	printf("\n");
}

void afisareListaStudenti(Nod* cap)
{
	while (cap)
	{
		afisareStudent(cap->info);
		cap = cap->next;
	}
}

//adaugam doar daca exista deja lista
void adaugaStudentInLista(Nod* cap,Student student)
{
	while (cap->next)
	{
		cap = cap->next;
	}
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = student;
	nod->next = NULL;
	cap->next = nod;
}
int calculeazaHash(int id, int dimensiune)
{
	return (id * 5) % dimensiune;
}
void inserareStudentInTabela(HashTable* ht, Student s)
{
	int pozitie = calculeazaHash(s.id, ht->dim);
	if (ht->vector[pozitie] == NULL)
	{
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = s;
		nod->next = NULL;
		ht->vector[pozitie] = nod;
	}
	else
	{
		adaugaStudentInLista(ht->vector[pozitie], s);
	}
}
HashTable initializare(int dimensiune)
{
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*)*dimensiune);
	//punem NULL
	for (int i = 0; i < ht.dim; i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}

HashTable citireStudentiDinFisier(const char* numeFisier,int dimensiune)
{
	FILE* file = fopen(numeFisier, "r");
	HashTable ht = initializare(dimensiune);
	if (file)
	{
		while (!feof(file))
		{
			Student st = citireStudentDinFiser(file);
			inserareStudentInTabela(&ht, st);
		}
		fclose(file);
	}
	return ht;
}
void afisareTabelaStudenti(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("Clusterul %d\n\n", i + 1);
			afisareListaStudenti(ht.vector[i]);
			printf("\n------------------\n");
		}
	}
}
Student getStudentDinLista(Nod* cap, int id)
{
	Student s;
	s.id = -1;
	while (cap != NULL && cap->info.id != id)
		cap = cap->next;
	if (cap != NULL)//am gasit
	{
		s = cap->info;
		s.nume = (char*)malloc(sizeof(char) *( strlen(cap->info.nume) + 1));
		strcpy(s.nume, cap->info.nume);
		s.specializare = (char*)malloc(sizeof(char) *( strlen(cap->info.specializare) + 1));
		strcpy(s.specializare, cap->info.specializare);

	}
	return s;
}
Student getStudentDupaId(HashTable ht, int id) {
	Student s;
	s.id = -1;

	int pozitie = calculeazaHash(id, ht.dim);

	if (pozitie >= 0 && pozitie < ht.dim)
	{
		return getStudentDinLista(ht.vector[pozitie], id);
	}
	return s;
}
float calculeazaMedieLista(Nod* cap)
{
	float suma = 0;
	int contor = 0;
	while (cap)
	{
		suma = suma + cap->info.medie;
		contor++;
		cap = cap->next;
	}
	if (contor > 0)
		return suma / contor;
	return 0;

}
float* calculeazaMediiPerCluster(HashTable ht, int* nrClustere) {
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
			(*nrClustere)++;
	}
	float* rezultat = (float*)malloc((*nrClustere) * sizeof(float));
	int j = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i]!=NULL)
		{
			rezultat[j] = calculeazaMedieLista(ht.vector[i]);
			j++;
		}
	}
	return rezultat;
}
void dezalocareLista(Nod** cap)
{
	while (*cap)
	{
		Nod* p = *cap;
		*cap = (*cap)->next;
		
		if (p->info.nume != NULL)
			free(p->info.nume);
		if (p->info.specializare != NULL)
			free(p->info.specializare);
		free(p);
	}
	*cap = NULL;
}
void dezalocareTabel(HashTable* ht)
{
	for (int i = 0; i < (*ht).dim; i++)
	{
		dezalocareLista(&ht->vector[i]);
	}
	//dezalocam blocul
	free(ht->vector);
	(*ht).dim = 0;
	(*ht).vector = NULL;
}
int main()
{
	HashTable ht = citireStudentiDinFisier("studenti.txt", 3);
	afisareTabelaStudenti(ht);

	Student s = getStudentDupaId(ht, 2);
	if (s.id != -1)
	{
		printf("Student gasit:\n");
		afisareStudent(s);
		free(s.nume);
		free(s.specializare);
	}

	int nrClustere;
	float* rez= calculeazaMediiPerCluster(ht, &nrClustere);

	for (int i = 0; i < nrClustere; i++)
	{
		printf("Media pentru clusterul %d este %.2f\n", i+1, rez[i]);
	}
	free(rez);


	dezalocareTabel(&ht);
	return 0;
}