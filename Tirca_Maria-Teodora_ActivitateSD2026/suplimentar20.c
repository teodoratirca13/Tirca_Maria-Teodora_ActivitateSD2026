#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

void afisareCarte(Carte carte)
{
	printf("Id:%d\n", carte.id);
	printf("Nr pagini:%d\n", carte.nrPagini);
	printf("Pret:%.2f\n", carte.pret);
	printf("Titlu:%s\n", carte.titlu);
	printf("Autor:%s\n", carte.autor);
	printf("Categorie:%c\n", carte.categorie);
	printf("\n");
}
Carte citireCarteDinFisier(FILE* file)
{
	char linie[200];
	fgets(linie, 200, file);
	char sep[3] = ",\n";

	Carte c;
	c.id = atoi(strtok(linie, sep));
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));

	char* aux;
	aux = strtok(NULL, sep);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);

	aux = strtok(NULL, sep);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);

	c.categorie = strtok(NULL, sep)[0];
	return c;
}
//structua Heap
struct Heap
{
	int lungime;
	Carte* vector;
	int nrCarti;
};
typedef struct Heap Heap;

//initializare Heap

Heap initializareHeap(int lungime)
{
	Heap heap;
	heap.lungime = lungime;
	heap.nrCarti = 0;
	//alocam spatiu pt cate elemente am primit
	heap.vector = (Carte*)malloc(sizeof(Carte) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod)
{
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	//facem verificarea de max heap
	int pozMax = pozitieNod;   //pp ca maximul dintre cele 3 elemente este fix nodul meu
	if (pozFiuSt < heap.nrCarti && heap.vector[pozMax].id < heap.vector[pozFiuSt].id)
	{
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < heap.nrCarti && heap.vector[pozMax].id < heap.vector[pozFiuDr].id)
	{
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod)
	{
		//interschimbam
		Carte aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrCarti - 2) / 2)  //este parinte
		{
			filtreazaHeap(heap, pozMax);
		}
	}
}
Heap citireHeapDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(7);
	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			heap.vector[heap.nrCarti] = carte;
			heap.nrCarti++;
		}
		fclose(file);
	}

	//aplicam filtrarea plecand de la ultimul parinte
	for (int i = (heap.nrCarti - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}
	return heap;
}
void afisareHeap(Heap heap)
{
	//cele vizibile
	for (int i = 0; i < heap.nrCarti; i++)
	{
		afisareCarte(heap.vector[i]);
	}
}
void afisareHeapAscuns(Heap heap)
{
	for (int i = heap.nrCarti; i < heap.lungime; i++)
	{
		afisareCarte(heap.vector[i]);
	}
}
Carte extragereCarte(Heap* heap)
{
	Carte c;
	c.id = -1;
	//elementul extrag este ascuns
	if (heap->nrCarti > 0)  //am ce extrage
	{
		c = heap->vector[0];

		//interschimbam cu finalul
		heap->vector[0] = heap->vector[heap->nrCarti - 1];
		heap->vector[heap->nrCarti - 1] = c;
		heap->nrCarti--;

		//filtram vectorul ramas plecand de la ultimul parinte
		for (int i = (heap->nrCarti - 2) / 2; i >= 0; i--)
		{
			filtreazaHeap(*heap, i);
		}
	}
	return c;
}
void dezalocareHeap(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		if (heap->vector[i].titlu)
		{
			free(heap->vector[i].titlu);
		}
		if (heap->vector[i].autor)
		{
			free(heap->vector[i].autor);
		}
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrCarti = 0;
}
int main()
{
	Heap heap = citireHeapDinFisier("carti.txt");
	afisareHeap(heap);
	printf("\nCarti extrase:\n");   //sortat descrescator
	afisareCarte(extragereCarte(&heap));
	afisareCarte(extragereCarte(&heap));
	afisareCarte(extragereCarte(&heap));
	afisareCarte(extragereCarte(&heap));
	afisareCarte(extragereCarte(&heap));
	afisareCarte(extragereCarte(&heap));
	afisareCarte(extragereCarte(&heap));

	printf("\nHeap ascuns:\n");  //sortat crescator
	afisareHeapAscuns(heap);

	dezalocareHeap(&heap);
	return 0;
}