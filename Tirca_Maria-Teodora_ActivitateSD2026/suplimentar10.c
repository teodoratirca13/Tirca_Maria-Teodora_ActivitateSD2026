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
Carte citireCarteDinFisier(FILE* file)
{
	char sep[3] = ",\n";
	char linie[100];
	fgets(linie, 100, file);

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

	c.categorie = (strtok(NULL, sep))[0];

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

struct Queue
{
	Carte* vector;
	int dim;
};
typedef struct Queue Queue;
void enqueue(Queue* queue, Carte carte)
{
	//punem la sfarsit
	Carte* aux = (Carte*)malloc(sizeof(Carte) * (queue->dim + 1));
	for (int i = 0; i < queue->dim; i++)
	{
		aux[i] = queue->vector[i];
	}
	aux[queue->dim] = carte;
	free(queue->vector);
	queue->vector = aux;
	queue->dim++;
}
Carte dequeue(Queue* queue)
{
	Carte carte;
	carte.id = -1;

	if (queue->dim > 0)
	{
		carte = queue->vector[0];
		Carte* aux = NULL;
		if (queue->dim > 1)
		{
			//refacem vectorul
			aux = (Carte*)malloc(sizeof(Carte) * (queue->dim - 1));
			for (int i = 1; i < queue->dim; i++)
			{
				aux[i - 1] = queue->vector[i];
			}
		}
		free(queue->vector);
		queue->vector = aux;
		queue->dim--;
	}
	return carte;
}
Queue citireCoadaDeCartiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Queue queue;
	queue.dim = 0;
	queue.vector = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			enqueue(&queue, carte);
		}
		fclose(file);
	}
	return queue;
}
void dezalocareCoada(Queue* queue)
{
	while (queue->vector != NULL)
	{
		Carte carte = dequeue(queue);
		if (carte.autor != NULL)
			free(carte.autor);
		if (carte.titlu != NULL)
			free(carte.titlu);
	}
}
Carte getCarteByIDQueue(Queue* queue, int id)
{
	Carte rezultat;
	rezultat.id = -1;
	Queue aux;
	aux.dim = 0;
	aux.vector = NULL;

	while (queue->vector)
	{
		Carte carte = dequeue(queue);
		if (carte.id == id)
		{
			rezultat = carte;
			rezultat.titlu = (char*)malloc(strlen(carte.titlu) + 1);
			strcpy(rezultat.titlu, carte.titlu);

			rezultat.autor = (char*)malloc(strlen(carte.autor) + 1);
			strcpy(rezultat.autor, carte.autor);
		}
		enqueue(&aux, carte);
	}
	//refacem coada initiala
	while (aux.vector)
	{
		enqueue(queue, dequeue(&aux));
	}
	return rezultat;
}
void afisareCoada(Queue queue)
{
	for (int i = 0; i < queue.dim; i++)
	{
		afisareCarte(queue.vector[i]);
	}
}
int main()
{
	Queue queue = citireCoadaDeCartiDinFisier("carti.txt");

	printf("Coada initiala:\n\n");
	afisareCoada(queue);

	printf("Carte cautata:\n\n");
	Carte carte = getCarteByIDQueue(&queue, 3);

	if (carte.id != -1)
	{
		afisareCarte(carte);
		free(carte.titlu);
		free(carte.autor);
	}
	else
	{
		printf("Nu exista o carte cu acest id.\n");
	}

	printf("\nCoada dupa cautare:\n\n");
	afisareCoada(queue);

	dezalocareCoada(&queue);

	return 0;
}