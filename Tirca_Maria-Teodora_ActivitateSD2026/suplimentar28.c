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

void afisareStudent(Student student)
{
	printf("Id: %d\n", student.id);
	printf("Nume: %s\n", student.nume);
	printf("An: %d\n", student.an);
	printf("Serie: %c\n", student.serie);
	printf("Specializare: %s\n", student.specializare);
	printf("Medie: %.2f\n", student.medie);
	printf("\n");
}

Student citireStudentDinFiser(FILE* file)
{
	char buffer[100];
	char separator[3] = ",\n";
	fgets(buffer, 100, file);
	Student s;
	s.id = atoi(strtok(buffer, separator));
	s.an = atoi(strtok(NULL, separator));
	s.medie = atof(strtok(NULL, separator));

	char* temp1 = strtok(NULL, separator);
	s.nume = (char*)malloc(strlen(temp1) + 1);
	strcpy(s.nume, temp1);

	char* temp2 = strtok(NULL, separator);
	s.specializare = (char*)malloc(strlen(temp2) + 1);
	strcpy(s.specializare, temp2);

	s.serie = strtok(NULL, separator)[0];
	return s;
}

typedef struct NodArbore NodArbore;
struct NodArbore {
	Student info;
	NodArbore* stanga;
	NodArbore* dreapta;
};

int calculeazaInaltimeInArbore(NodArbore* radacina)
{
	if (radacina == NULL)
	{
		return 0;
	}
	int inaltimeSt = calculeazaInaltimeInArbore(radacina->stanga);
	int inaltimeDr = calculeazaInaltimeInArbore(radacina->dreapta);
	if (inaltimeDr > inaltimeSt)
	{
		return 1 + inaltimeDr;
	}
	else
	{
		return 1 + inaltimeSt;
	}
}
void inserareStudentInArbore(NodArbore** radacina,Student student)
{
	if (*radacina == NULL)
	{
		NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
		nod->info = student;
		nod->dreapta = NULL;
		nod->stanga = NULL;
		*radacina = nod;
	}
	else
	{
		if (student.id > (*radacina)->info.id)
		{
			inserareStudentInArbore(&(*radacina)->dreapta, student);
		}
		else
		{
			inserareStudentInArbore(&(*radacina)->stanga, student);
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
			Student student = citireStudentDinFiser(file);
			inserareStudentInArbore(&radacina, student);
		}
		fclose(file);
	}
	return radacina;
}

void afisareArborePreordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareStudent(radacina->info);
		afisareArborePreordine(radacina->stanga);
		afisareArborePreordine(radacina->dreapta);
	}
}
void afisareArboreInordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareArboreInordine(radacina->stanga);
		afisareStudent(radacina->info);
		afisareArboreInordine(radacina->dreapta);
	}
}
void afisareArborePostordine(NodArbore* radacina)
{
	if (radacina)
	{
		afisareArborePostordine(radacina->stanga);
		afisareArborePostordine(radacina->dreapta);
		afisareStudent(radacina->info);
	}
}
void dezalocareAbore(NodArbore** radacina)
{
	if (*radacina)
	{
		dezalocareAbore(&(*radacina)->dreapta);
		dezalocareAbore(&(*radacina)->stanga);

		if ((*radacina)->info.nume)
		{
			free((*radacina)->info.nume);
		}
		if ((*radacina)->info.specializare)
		{
			free((*radacina)->info.specializare);
		}
		free(*radacina);
		*radacina = NULL;
	}
}
Student getStudentById(NodArbore* radacina, int idCautat)
{
	if (radacina != NULL)
	{
		if (radacina->info.id == idCautat)
		{
			Student s = radacina->info;
			s.nume = (char*)malloc(strlen(radacina->info.nume) + 1);
			strcpy(s.nume, radacina->info.nume);
			s.specializare = (char*)malloc(strlen(radacina->info.specializare) + 1);
			strcpy(s.specializare, radacina->info.specializare);

			return s;
		}
		else
		{
			if (radacina->info.id < idCautat)
			{
				return getStudentById(radacina->dreapta, idCautat);
			}
			else
			{
				return getStudentById(radacina->stanga, idCautat);
			}
		}
	}
	Student s;
	s.id = -1;
	s.nume = NULL;
	s.specializare = NULL;
	return s;
}
int determinaNumarNoduri(NodArbore* radacina)
{
	if (radacina==NULL)
	{
		return 0;
	}
	return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}
int main()
{
	NodArbore* radacina = citireArboreDinFisier("studenti_arbore.txt");
	printf("Afisare inordine:\n");
	afisareArboreInordine(radacina);

	printf("Afisare preordine:\n");
	afisareArborePreordine(radacina);

	printf("Afisare postordine:\n");
	afisareArborePostordine(radacina);

	Student s = getStudentById(radacina, 5);

	if (s.id != -1)
	{
		printf("\nStudent cautat:\n");
		afisareStudent(s);

		free(s.nume);
		free(s.specializare);
	}
	else
	{
		printf("Nu exista student cu acest id!\n");
	}

	printf("Nr noduri: %d\n", determinaNumarNoduri(radacina));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeInArbore(radacina));
	return 0;
}