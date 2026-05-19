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

struct Heap
{
	Angajat* vector;
	int lungime; //inclusiv cei ascunsi
	int nr_angajati;
};
typedef struct Heap Heap;

Heap initializareHeap(int lungime)
{
	Heap heap;
	heap.vector = (Angajat*)malloc(sizeof(Angajat) * lungime);
	heap.lungime = lungime;
	heap.nr_angajati = 0;
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod)
{
	int pozFiuSt = pozitieNod * 2 + 1;
	int pozFiuDr = pozitieNod * 2 + 2;

	int pozMin = pozitieNod;
	if (heap.vector[pozFiuDr].id < heap.vector[pozMin].id && pozFiuDr < heap.nr_angajati)
	{
		pozMin = pozFiuDr;
	}
	if (heap.vector[pozFiuSt].id < heap.vector[pozMin].id && pozFiuSt < heap.nr_angajati)
	{
		pozMin = pozFiuSt;
	}

	if (pozMin != pozitieNod) //presupunerea a fost gresit
	{
		//interschimbam

		Angajat aux = heap.vector[pozMin];
		heap.vector[pozMin] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMin <= (heap.nr_angajati - 2) / 2)
		{
			filtreazaHeap(heap, pozMin);
		}
	}
}

Heap citireHeapDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	if (file)
	{
		while (!feof(file))
		{
			Angajat angajat = citireAngajatDinFisier(file);
			heap.vector[heap.nr_angajati] = angajat;
			(heap.nr_angajati)++;
		}
		fclose(file);
	}
	//filtrarea
	for (int i = (heap.nr_angajati - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}
	return heap;
}
void afisareHeap(Heap heap)
{
	for (int i = 0; i < heap.nr_angajati; i++)
	{
		afisareAngajat(heap.vector[i]);
	}
}
void afisareHeapAscuns(Heap heap)
{
	for (int i = heap.nr_angajati; i < heap.lungime; i++)
	{
		afisareAngajat(heap.vector[i]);
	}
}

Angajat extragereAngajat(Heap* heap)
{
	Angajat angajat;
	angajat.id = -1;
	if (heap->nr_angajati > 0)
	{
		angajat= heap->vector[0];
		heap->vector[0] = heap->vector[heap->nr_angajati - 1];
		heap->vector[heap->nr_angajati - 1] = angajat;

		heap->nr_angajati--;

		//filtram
		for (int i = (heap->nr_angajati - 2) / 2; i >= 0; i--)
		{
			filtreazaHeap(*heap, i);
		}
	}
	return angajat;
}

void dezalocareHeap(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		if (heap->vector[i].nume)
		{
			free(heap->vector[i].nume);
		}
		if (heap->vector[i].functie)
		{
			free(heap->vector[i].functie);
		}
	}
	free(heap->vector);
	heap->vector = 0;
	heap->lungime = 0;
	heap->nr_angajati = 0;
}
int main()
{
	Heap heap = citireHeapDinFisier("angajati.txt");
	afisareHeap(heap);

	printf("\nAngajati extrasi:\n");

	afisareAngajat(extragereAngajat(&heap));
	afisareAngajat(extragereAngajat(&heap));
	afisareAngajat(extragereAngajat(&heap));
	afisareAngajat(extragereAngajat(&heap));
	afisareAngajat(extragereAngajat(&heap));
	afisareAngajat(extragereAngajat(&heap));
	afisareAngajat(extragereAngajat(&heap));
	afisareAngajat(extragereAngajat(&heap));
	afisareAngajat(extragereAngajat(&heap));
	afisareAngajat(extragereAngajat(&heap));

	printf("\nHeap ascuns:\n");
	afisareHeapAscuns(heap);

	dezalocareHeap(&heap);
	return 0;
}