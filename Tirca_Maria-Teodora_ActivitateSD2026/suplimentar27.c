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
struct Heap
{
	Student* vector;
	int lungime;
	int nr_studenti;
};
typedef struct Heap Heap;

Heap initializareHeap(int lungime)
{
	Heap heap;
	heap.lungime = lungime;
	heap.vector = (Student*)malloc(sizeof(Student) * heap.lungime);
	heap.nr_studenti = 0;
	return heap;
}
void filtrareHeap(Heap heap, int pozNod)
{
	int pozFiuSt = pozNod * 2 + 1;
	int pozFiuDr = pozNod * 2 + 2;

	int pozMax = pozNod;
	if (pozFiuDr < heap.nr_studenti && heap.vector[pozFiuDr].id > heap.vector[pozMax].id )
	{
		pozMax = pozFiuDr;
	}
	if (pozFiuSt < heap.nr_studenti && heap.vector[pozFiuSt].id > heap.vector[pozMax].id)
	{
		pozMax = pozFiuSt;
	}

	if (pozMax != pozNod)
	{
		//interschimbam
		Student aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozNod];
		heap.vector[pozNod] = aux;

		if (pozMax <= (heap.nr_studenti - 2) / 2) //daca e parinte
		{
			filtrareHeap(heap, pozMax);
		}
	}
}
Heap citireHeapDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(6);
	if (file)
	{
		while (!feof(file))
		{
			Student student = citireStudentDinFiser(file);
			heap.vector[heap.nr_studenti] = student;
			heap.nr_studenti++;
		}
		fclose(file);
	}

	//filtrare
	for (int i = (heap.nr_studenti - 2) / 2; i >= 0; i--)
	{
		filtrareHeap(heap, i);
	}
	return heap;
}
void afisareHeap(Heap heap)
{
	for (int i = 0; i < heap.nr_studenti; i++)
	{
		afisareStudent(heap.vector[i]);
	}
}
void afisareHeapAscuns(Heap heap)
{
	for (int i = heap.nr_studenti; i < heap.lungime; i++)
	{
		afisareStudent(heap.vector[i]);
	}
}
Student extragereStudent(Heap* heap)
{
	Student s;
	s.id = -1;
	s.nume = NULL;
	s.specializare = NULL;
	if ((*heap).nr_studenti > 0)
	{
		s = (*heap).vector[0];
		(*heap).vector[0] = (*heap).vector[(*heap).nr_studenti - 1];
		(*heap).vector[(*heap).nr_studenti - 1] = s;

		(*heap).nr_studenti--;

		//filtram
		for (int i = ((*heap).nr_studenti - 2) / 2; i >= 0; i--)
		{
			filtrareHeap(*heap, i);
		}
	}
	return s;
}
void dezalocareHeap(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		if (heap->vector[i].nume)
		{
			free(heap->vector[i].nume);
		}

		if (heap->vector[i].specializare)
		{
			free(heap->vector[i].specializare);
		}
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nr_studenti = 0;
}
int main()
{
	Heap heap = citireHeapDinFisier("studenti.txt");

	printf("Heap studenti:\n");
	afisareHeap(heap);

	printf("\nStudenti extrasi:\n");

	afisareStudent(extragereStudent(&heap));
	afisareStudent(extragereStudent(&heap));
	afisareStudent(extragereStudent(&heap));
	afisareStudent(extragereStudent(&heap));
	afisareStudent(extragereStudent(&heap));
	afisareStudent(extragereStudent(&heap));

	printf("\nHeap ascuns:\n");
	afisareHeapAscuns(heap);

	dezalocareHeap(&heap);

	return 0;
}