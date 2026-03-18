#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
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

typedef struct Nod Nod;
struct Nod
{
	Student info;
	Nod* next;
};
Student citireStudentDinFiser(FILE* file)
{
   //trebuie citita o linie din fisier cu fgets
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
	while (cap != NULL)
	{
		afisareStudent(cap->info);
		cap = cap->next;
	}
}
//la final
void adaugaStudentInLista(Nod** cap, Student studentNou)
{
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = studentNou;
	nodNou->next = NULL;

	if ((*cap) == NULL) //lista e goala
	{
		*cap = nodNou;
	}
	else {
		Nod* aux = *cap;
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nodNou;
	}
}
void adaugaLaInceputInLista(Nod** cap, Student studentNou)
{
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = studentNou;
	nodNou->next = (*cap);
	(*cap) = nodNou;	
}

Nod* citireListaStudentiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	if (file)
	{
		Nod* cap = NULL;
		while (!feof(file))
		{
			adaugaStudentInLista(&cap, citireStudentDinFiser(file));
		}
		fclose(file);
		return cap;
	}
	return NULL;
}
void dezalocareListaStudenti(Nod** cap)
{
	//aici merge cu cap ca oricum se face NULL la final
	while (*cap != NULL)
	{
		Nod* p = *cap;
		(*cap) = p->next;
		if (p->info.nume != NULL)
		{
			free(p->info.nume);
		}
		if (p->info.specializare != NULL)
		{
			free(p->info.specializare);
		}
		free(p);
    }
}
float calculeazaMedieGeneralaLista(Nod* cap)
{
	float suma = 0;
	int contor = 0;
	while (cap != NULL)
	{
		suma += cap->info.medie;
		contor++;
		cap = cap->next;
	}
	if (contor > 0)
		return suma / contor;
	return 0;
}
float calculeazaMediaPeSpecializare(Nod* cap, const char* specializareCautata)
{
	float suma = 0;
	int contor = 0;
	while (cap != NULL)
	{
		if (strcmp(cap->info.specializare, specializareCautata) == 0)
		{
			suma += cap->info.medie;
			contor++;
		}
		cap = cap->next;
	}
	if (contor > 0)
		return suma / contor;
	return 0;
}
void stergeStudentiDinAnul(Nod** cap, int anCautat)
{
	//stergem de la inceput
	while ((*cap) != NULL && (*cap)->info.an == anCautat)
	{
		Nod* aux = *cap;
		(*cap) = aux->next;
		if (aux->info.nume != NULL)
		{
			free(aux->info.nume);
		}
		if (aux->info.specializare != NULL)
		{
			free(aux->info.specializare);
		}
		free(aux);
	}
	if ((*cap)!=NULL) //daca mai am lista dupa ce le-am sters pe cele initiale
	{
		Nod* p = *cap;
		while (p)
		{
			while (p->next != NULL && p->next->info.an != anCautat)
			{
				p = p->next;
			}
			if (p->next)
			{
				Nod* aux = p->next;
				p->next = aux->next;

				if (aux->info.nume != NULL)
				{
					free(aux->info.nume);
				}
				if (aux->info.specializare != NULL)
				{
					free(aux->info.specializare);
				}
				free(aux);
			}
			else
			{
				p = NULL;
			}
		}
	}
}
int main()
{
	Nod* cap = NULL;
	cap=citireListaStudentiDinFisier("studenti.txt");
	printf("Lista initiala:\n");
	afisareListaStudenti(cap);

	printf("Media generala: %.2f\n", calculeazaMedieGeneralaLista(cap));
	printf("Media la Informatica Economica: %.2f\n", calculeazaMediaPeSpecializare(cap, "Informatica Economica"));

	stergeStudentiDinAnul(&cap, 2);
	printf("\nDupa stergerea studentilor din anul 2:\n");
	afisareListaStudenti(cap);

	dezalocareListaStudenti(&cap);
	return 0;
}