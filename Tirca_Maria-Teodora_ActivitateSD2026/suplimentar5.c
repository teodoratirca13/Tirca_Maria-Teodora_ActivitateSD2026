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

struct Nod
{
	Student info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla
{
	Nod* primul;
	Nod* ultimul;
};
typedef struct ListaDubla ListaDubla;

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

void afisareListaStudenti(ListaDubla lista)
{
	Nod* aux = lista.primul;
	while (aux != NULL)
	{
		afisareStudent(aux->info);
		aux = aux->next;
	}
}
void afisareListaStudentiDeLaSfarsit(ListaDubla lista)
{
	Nod* aux = lista.ultimul;
	while (aux)
	{
		afisareStudent(aux->info);
		aux=aux->prev;
	}
}
void adaugaStudentInLista(Student studentNou, ListaDubla* lista)
{
	//la final;
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = studentNou;
	nodNou->next = NULL;
	nodNou->prev = lista->ultimul;
	if (lista->ultimul != NULL) //lista nu e goala
	{
		lista->ultimul->next = nodNou;
	}
	else
	{
		lista->primul = nodNou;
	}
	lista->ultimul = nodNou;
}
void adaugaStudentInListaLaInceput(Student studentNou, ListaDubla* lista)
{
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = studentNou;
	nodNou->prev = NULL;
	nodNou->next = lista->primul;
	if (lista->primul != NULL) //lista nu e goala
	{
		lista->primul->prev = nodNou;
	}
	else
	{
		lista->ultimul = nodNou;
	}
	lista->primul = nodNou;
}
ListaDubla citireLDStudentiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.primul = NULL;
	lista.ultimul = NULL;
	if(file)
	{
		while (!feof(file))
		{
			adaugaStudentInLista(citireStudentDinFiser(file), &lista);
		}
		fclose(file);
	}
	return lista;
}
void dezalocareLDStudenti(ListaDubla* lista)
{
	Nod* p = lista->primul;
	while (p)
	{
		Nod* aux = p;
		p = p->next;
		if (aux->info.specializare != NULL)
			free(aux->info.specializare);
		if (aux->info.nume != NULL)
			free(aux->info.nume);
		free(aux);
	}
	lista->primul = NULL;
	lista->ultimul = NULL;
}
float calculeazaMedieGenerala(ListaDubla lista)
{
	Nod* aux = lista.primul;
	float suma = 0;
	int contor = 0;
	while (aux)
	{
		suma += aux->info.medie;
		contor++;
		aux = aux->next;
	}
	if (contor != 0)
	{
		return suma / contor;
	}
	return 0;
}
char* getNumeStudentCuMediaMaxima(ListaDubla lista)
{
	if (lista.primul) //daca lista nu e goala
	{
		Nod* Max = lista.primul;
		Nod* p = lista.primul;
		while (p)
		{
			if (p->info.medie > Max->info.medie)
			{
				Max = p;
			}
			p = p->next;
		}
		char* nume = (char*)malloc(strlen(Max->info.nume) + 1);
		strcpy(nume, Max->info.nume);
		return nume;
	}
	return NULL;
}
void stergeStudentDupaID(ListaDubla* lista, int id)
{
	if (lista->primul == NULL)
	{
		return;
	}
	Nod* p = lista->primul;
	while (p && p->info.id != id)
	{
		p = p->next;
	}
	if (p == NULL) //daca am iesit din lista
		return;

	if (p->prev == NULL) //e primul nod
	{
		lista->primul = p->next;
		if (p->next != NULL)
		{
			p->next->prev = NULL;
		}
	}
	else
	{
		p->prev->next = p->next;
	}

	if (p->next == NULL) //este ultimul nod
	{
		lista->ultimul = p->prev;
	}
	else
	{
		p->next->prev = p->prev;
	}

	//stergera
	if (p->info.specializare != NULL)
		free(p->info.specializare);
	if (p->info.nume != NULL)
		free(p->info.nume);
	free(p);
}
int main()
{
	ListaDubla lista = citireLDStudentiDinFisier("studenti.txt");

	printf("lista initiala de la inceput:\n");
	afisareListaStudenti(lista);
	printf("lista initiala de la sfarsit:\n");
	afisareListaStudentiDeLaSfarsit(lista);

	printf("media generala a studentilor este: %.2f\n", calculeazaMedieGenerala(lista));

	char* nume = getNumeStudentCuMediaMaxima(lista);
	if (nume != NULL)
	{
		printf("\nStudentul cu media cea mai mare: %s\n\n", nume);
		free(nume);
	}
	stergeStudentDupaID(&lista, 2);
	printf("lista dupa stergerea studentului cu id-ul 2:\n");
	afisareListaStudenti(lista);

	dezalocareLDStudenti(&lista);
	return 0;
}
