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
	char linie[100];
	fgets(linie, 100, file);

	char separator[3] = ",\n";
	Carte c;

	c.id = atoi(strtok(linie, separator));
	c.nrPagini = atoi(strtok(NULL, separator));
	c.pret = atof(strtok(NULL, separator));

	char* aux;
	aux = strtok(NULL, separator);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);

	aux = strtok(NULL, separator);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);

	c.categorie = strtok(NULL, separator)[0];
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
struct Stiva
{
	int dim;
	Carte* vector;
};
typedef struct Stiva Stiva;

void pushStack(Stiva* stiva, Carte carte)
{
	//punem la sfarist
	Carte* aux = (Carte*)malloc(sizeof(Carte) * (stiva->dim + 1));
	for (int i = 0; i < stiva->dim; i++)
	{
		aux[i] = stiva->vector[i];
	}
	aux[stiva->dim] = carte;
	free(stiva->vector);
	stiva->vector = aux;
	(stiva->dim)++;
}
Carte popStack(Stiva* stiva)
{
	Carte carte;
	carte.id = -1;
	carte.titlu = NULL;
	carte.autor = NULL;
	if (stiva->dim > 0)
	{
		carte = stiva->vector[stiva->dim - 1];
		Carte* aux = NULL;
		if (stiva->dim > 1)
		{
			//refacem vectorul
			aux = (Carte*)malloc(sizeof(Carte) * (stiva->dim - 1));
			for (int i = 0; i < stiva->dim - 1; i++)
			{
				aux[i] = stiva->vector[i];
			}
		}
		free(stiva->vector);
		stiva->vector = aux;
		stiva->dim--;
	}
	return carte;
}
char emptyStack(Stiva stiva)
{
	return stiva.dim == 0;
}
Stiva citireStackDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Stiva stack;
	stack.dim = 0;
	stack.vector = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			pushStack(&stack, carte);
		}
		fclose(file);
	}
	return stack;
}
void dezalocareStiva(Stiva* stiva)
{
	/*for (int i = 0; i < stiva->dim; i++)
	{
		if (stiva->vector[i].titlu)
			free(stiva->vector[i].titlu);
		if (stiva->vector[i].autor)
			free(stiva->vector[i].autor);
	}
	free(stiva->vector);
	stiva->vector = NULL;
	stiva->dim = 0;*/

	while (!emptyStack(*stiva))
	{
		Carte carte = popStack(stiva);
		free(carte.titlu);
		free(carte.autor);
	}
	stiva->vector = NULL;
	stiva->dim = 0;
}
Carte getCarteByIDStiva(Stiva* stiva, int id)
{
	Carte rezultat;
	rezultat.id = -1;
	rezultat.autor = NULL;
	rezultat.titlu = NULL;
	Stiva aux;
	aux.dim = 0;
	aux.vector = NULL;
	while (!emptyStack(*stiva))
	{
		Carte carte = popStack(stiva);
		if (carte.id == id)
		{
			rezultat = carte;
			rezultat.titlu = (char*)malloc(strlen(carte.titlu) + 1);
			strcpy(rezultat.titlu, carte.titlu);

			rezultat.autor = (char*)malloc(strlen(carte.autor) + 1);
			strcpy(rezultat.autor, carte.autor);
		}
		pushStack(&aux, carte);
	}
	while (!emptyStack(aux))
	{
		pushStack(stiva, popStack(&aux));
	}
	return rezultat;
}
int size(Stiva* stiva)
{
	Stiva aux;
	aux.dim = 0;
	aux.vector = NULL;
	int contor = 0;
	while (!emptyStack(*stiva))
	{
		Carte carte = popStack(stiva);
		contor++;
		pushStack(&aux, carte);
	}
	while (!emptyStack(aux))
	{
		pushStack(stiva, popStack(&aux));
	}
	return contor;
}
float calculeazaPretTotalStiva(Stiva* stiva)
{
	float suma = 0;
	Stiva aux;
	aux.dim = 0;
	aux.vector = NULL;
	while (!emptyStack(*stiva))
	{
		Carte carte = popStack(stiva);
		suma += carte.pret;
		pushStack(&aux, carte);
	}
	while (!emptyStack(aux))
	{
		pushStack(stiva, popStack(&aux));
	}
	return suma;
}
int main()
{
	Stiva stiva = citireStackDinFisier("carti.txt");

	printf("Continutul initial al stivei:\n\n");
	for (int i = stiva.dim - 1; i >= 0; i--)
	{
		afisareCarte(stiva.vector[i]);
	}

	printf("Dimensiunea stivei este: %d\n", size(&stiva));

	float pretTotal = calculeazaPretTotalStiva(&stiva);
	printf("\nPretul total al cartilor este: %.2f\n", pretTotal);

	Carte carte = getCarteByIDStiva(&stiva, 3);
	if (carte.id != -1)
	{
		printf("\nCartea gasita dupa ID:\n");
		afisareCarte(carte);

		free(carte.titlu);
		free(carte.autor);
	}
	else
	{
		printf("\nNu exista carte cu acest ID.\n");
	}

	printf("\nScoatem ultimul element din stiva:\n");
	Carte carteScoasa = popStack(&stiva);
	if (carteScoasa.id != -1)
	{
		afisareCarte(carteScoasa);

		free(carteScoasa.titlu);
		free(carteScoasa.autor);
	}

	printf("\nStiva dupa pop:\n\n");
	for (int i = stiva.dim - 1; i >= 0; i--)
	{
		afisareCarte(stiva.vector[i]);
	}

	dezalocareStiva(&stiva);

	return 0;
}