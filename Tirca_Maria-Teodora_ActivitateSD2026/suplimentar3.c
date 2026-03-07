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
void afisareVectorStudenti(Student* studenti, int nrStudenti)
{
	if (studenti == NULL || nrStudenti == 0)
	{
		printf("Vectorul de studenti este gol!\n");
		return;
	}
	for (int i = 0; i < nrStudenti; i++)
	{
		afisareStudent(studenti[i]);
	}
}
void adaugareStudentInVector(Student** studenti, int* nrStudenti, Student studentDeAdaugat)
{
	Student* temp = (Student*)malloc(sizeof(Student) * ((*nrStudenti) + 1));
	for (int i = 0; i < *nrStudenti; i++)
	{
		temp[i] =(*studenti)[i]; // e ok aici shallow copy
	}
	temp[*nrStudenti] = studentDeAdaugat;
	free(*studenti);
	 *studenti = temp; //acum amandoi pointeaza catre aceeasi zona de memorie
	(*nrStudenti)++;
	//temp dispare la finalul functiei, deoarece e pe stiva acesteia
}
Student citireStudentDinFisier(FILE* file)
{
	Student s;
	char linie[100];
	fgets(linie, 100, file); //fgets citeste o linie
	char separator[3] = ",\n";

	s.id = atoi(strtok(linie, separator));
	s.an= atoi(strtok(NULL, separator));
	s.medie= atof(strtok(NULL, separator));

	char* temp1 = strtok(NULL, separator);
	s.nume = (char*)malloc(strlen(temp1) + 1);
	strcpy(s.nume, temp1);

	char* temp2 = strtok(NULL, separator);
	s.specializare = (char*)malloc(strlen(temp2) + 1);
	strcpy(s.specializare, temp2);

	s.serie = strtok(NULL, separator)[0]; //index face deplasare si dereferentiere
	return s;
}
Student* construireVectorStudenti(const char* numeFisier, int* nrStudentiCititi)
{
	FILE* file = fopen(numeFisier, "r");
	if (!file)
	{
		printf("Eroare la deschiderea fisierului!\n");
		return NULL;
	}
	else {
		Student* vectorStudenti = NULL;
		*nrStudentiCititi = 0;
		while (!feof(file))  //cat timp nu am ajuns la sfarsitul fisierului
		{
			Student s = citireStudentDinFisier(file);
			adaugareStudentInVector(&vectorStudenti,nrStudentiCititi,s);
		}
		//inchidem fisierul
		fclose(file);
		return vectorStudenti;
	}
}

Student* filtrareMedie(Student* studenti, int nrStudenti, int* dimensiune)
{
	Student* rezultat = NULL;
	*dimensiune = 0;
	for (int i = 0; i < nrStudenti; i++)
	{
		if (studenti[i].medie > 8)
		{
			adaugareStudentInVector(&rezultat, dimensiune, studenti[i]);
		}
	}
	return rezultat;
}

//Student* cautaStudentDupaId(Student* studenti, int nrStudenti, int id)
//{
//	Student* stud = NULL;
//	for (int i = 0; i < nrStudenti; i++)
//	{
//		if (studenti[i].id == id)
//		{
//			stud = &studenti[i];
//		}
//	}
//	return stud;
//}
Student cautaStudentDupaId(Student* studenti, int nrStudenti, int id)
{
	for (int i = 0; i < nrStudenti; i++)
	{
		if (studenti[i].id == id)
		{
			return studenti[i];
		}
	}
	Student gol;
	gol.id = -1;
	return gol;
}
void dezalocareVectorStudenti(Student** vector, int* nrStudenti) {
	if (vector == NULL || *vector == NULL || nrStudenti == NULL || *nrStudenti == 0) {
		return;
	}

	for (int i = 0; i < *nrStudenti; i++) {
		free((*vector)[i].nume);
		(*vector)[i].nume = NULL;

		free((*vector)[i].specializare);
		(*vector)[i].specializare = NULL;
	}

	free(*vector);
	*vector = NULL;
	*nrStudenti = 0;
}
int main()
{
	int nrStudenti = 0;
	Student* studenti = construireVectorStudenti("studenti.txt", &nrStudenti);
	afisareVectorStudenti(studenti, nrStudenti);

	int dimensiune = 0;
	Student* rez = filtrareMedie(studenti, nrStudenti, &dimensiune);
	printf("Dupa filitrare medie peste8:\n");
	afisareVectorStudenti(rez, dimensiune);

	free(rez);
	rez = NULL;
	dimensiune = 0;

	//Student* s = cautaStudentDupaId(studenti, nrStudenti, 3);

	//if (s != NULL)
	//{
	//	printf("Student gasit:\n");
	//	afisareStudent(*s); //dereferentiem s;
	//}
	//else
	//{
	//	printf("Studentul nu exista\n");
	//}

	Student s = cautaStudentDupaId(studenti, nrStudenti, 5);
	if (s.id != -1)
	{
		printf("Student gasit:\n");
		afisareStudent(s);
	}
	else
	{
		printf("Studentul nu exista\n");
	}

	dezalocareVectorStudenti(&studenti, &nrStudenti);

	return 0;
}