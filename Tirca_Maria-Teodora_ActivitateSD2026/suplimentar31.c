#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct StructuraTelefon
{
	int id;
	int memorie;
	float pret;
	char* model;
	char* producator;
	unsigned char categorie;
};
typedef struct StructuraTelefon Telefon;

void afisareTelefon(Telefon telefon)
{
	printf("Id:%d\n", telefon.id);
	printf("Memorie:%d GB\n", telefon.memorie);
	printf("Pret:%.2f\n", telefon.pret);
	printf("Model:%s\n", telefon.model);
	printf("Producator:%s\n", telefon.producator);
	printf("Categorie:%c\n", telefon.categorie);
	printf("\n");
}

Telefon citireTelefonDinFisier(FILE* file)
{
	char linie[200];
	fgets(linie, 200, file);
	char sep[3] = ",\n";

	Telefon t;

	t.id = atoi(strtok(linie, sep));
	t.memorie = atoi(strtok(NULL, sep));
	t.pret = atof(strtok(NULL, sep));

	char* aux;

	aux = strtok(NULL, sep);
	t.model = (char*)malloc(strlen(aux) + 1);
	strcpy(t.model, aux);

	aux = strtok(NULL, sep);
	t.producator = (char*)malloc(strlen(aux) + 1);
	strcpy(t.producator, aux);

	t.categorie = strtok(NULL, sep)[0];

	return t;
}
struct Heap
{
	Telefon* vector;
	int lungime;
	int nr_telefoane;
};
typedef struct Heap Heap;

Heap initializareHeap(int lungime)
{
	Heap heap;
	heap.lungime = lungime;
	heap.nr_telefoane = 0;
	heap.vector = (Telefon*)malloc(sizeof(Telefon) * heap.lungime);
	return heap;
}

void filtrareHeap(Heap heap, int pozNod)
{
	int pozFiuSt = 2 * pozNod + 1;
	int pozFiuDr = 2 * pozNod + 2;
	int pozMax = pozNod;

	if (pozFiuSt<heap.nr_telefoane && heap.vector[pozFiuSt].id>heap.vector[pozMax].id)
	{
		pozMax = pozFiuSt;
	}
	if (pozFiuDr<heap.nr_telefoane && heap.vector[pozFiuDr].id>heap.vector[pozMax].id)
	{
		pozMax = pozFiuDr;
	}
	if (pozMax != pozNod)
	{
		//interschimbam
		Telefon aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozNod];
		heap.vector[pozNod] = aux;

		if (pozMax <= (heap.nr_telefoane - 2) / 2)
		{
			filtrareHeap(heap, pozMax);
		}
	}
}
Heap citireHeapDinFisier(const char* numeFisier)
{
	Heap heap = initializareHeap(7);
	FILE* file = fopen(numeFisier, "r");
	if (file)
	{
		while (!feof(file))
		{
			Telefon t = citireTelefonDinFisier(file);
			heap.vector[heap.nr_telefoane] = t;
			heap.nr_telefoane++;
		}
		fclose(file);
	}
	//filtram
	for (int i = (heap.nr_telefoane - 2) / 2; i >= 0; i--)
	{
		filtrareHeap(heap, i);
	}
	return heap;
}
void afisareHeap(Heap heap)
{
	for (int i = 0; i < heap.nr_telefoane; i++)
	{
		afisareTelefon(heap.vector[i]);
	}
}
void afisareHeapAscuns(Heap heap)
{
	for (int i = heap.nr_telefoane; i < heap.lungime; i++)
	{
		afisareTelefon(heap.vector[i]);
	}
}
Telefon extragereTelefon(Heap* heap)
{
	Telefon t;
	t.id = -1;
	t.model = NULL;
	t.producator = NULL;
	if ((*heap).nr_telefoane > 0)
	{
		//interschimbam
		t = (*heap).vector[0];
		(*heap).vector[0] = (*heap).vector[heap->nr_telefoane - 1];
		(*heap).vector[heap->nr_telefoane - 1] = t;
		(*heap).nr_telefoane--;

		//filtram
		for (int i = ((*heap).nr_telefoane - 2) / 2; i >= 0; i--)
		{
			filtrareHeap(*heap, i);
		}
	}
	return t;
}
void dezalocareHeap(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		if (heap->vector[i].model)
		{
			free(heap->vector[i].model);
		}
		if (heap->vector[i].producator)
		{
			free(heap->vector[i].producator);
		}
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nr_telefoane = 0;
}
int main()
{
	Heap heap = citireHeapDinFisier("telefoane.txt");

	printf("Heap telefoane:\n");
	afisareHeap(heap);

	printf("\nTelefoane extrase:\n");

	afisareTelefon(extragereTelefon(&heap));
	afisareTelefon(extragereTelefon(&heap));
	afisareTelefon(extragereTelefon(&heap));
	afisareTelefon(extragereTelefon(&heap));
	afisareTelefon(extragereTelefon(&heap));
	afisareTelefon(extragereTelefon(&heap));
	afisareTelefon(extragereTelefon(&heap));

	printf("\nHeap ascuns:\n");
	afisareHeapAscuns(heap);

	dezalocareHeap(&heap);

	return 0;
}