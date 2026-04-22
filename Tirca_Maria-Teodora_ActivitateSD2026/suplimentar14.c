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

//stack prin ldi
struct Nod
{
	Carte info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct Stack
{
	Nod* start;
	Nod* end;
};
typedef struct Stack Stack;

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
void pushStack(Stack* stack, Carte carte)
{
	//adaugam la final
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = carte;
	nod->next = NULL;
	nod->prev = (*stack).end;

	if (stack->end == NULL)
	{
		stack->start = nod;
	}
	else
	{
		stack->end->next = nod;
	}
	stack->end = nod;
}
Carte popStack(Stack* stack)
{
	//extragem de la final;
	Carte carte;
	carte.id = -1;
	if (stack->end != NULL) //avem ce sterge
	{
		carte = stack->end->info;
		stack->end = stack->end->prev;
		if (stack->end != NULL)
		{
			free(stack->end->next);
			stack->end->next = NULL;
		}
		else //avem doar un element
		{
			free(stack->start);
			stack->start = NULL;
		}
	}
	return carte;
}
char emptyStack(Stack stack)
{
	return stack.end == NULL;
}
Stack citireStivaDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Stack stack;
	stack.end = NULL;
	stack.start = NULL;
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
void afisareCarte(Carte carte)
{
	printf("Id: %d\n", carte.id);
	printf("Nr. pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Categorie: %c\n\n", carte.categorie);
}
void dezalocare(Stack* stack)
{
	while (stack->end)
	{
		Carte carte = popStack(stack);
		if (carte.autor != NULL)
			free(carte.autor);
		if (carte.titlu != NULL)
			free(carte.titlu);
	}
	stack->end = NULL;
	stack->start = NULL;
}
Carte getCarteByIDStack(Stack* stack, int id)
{
	Carte rezultat;
	rezultat.id = -1;
	Stack aux;
	aux.end = NULL;
	aux.start = NULL;

	while (stack->end)
	{
		Carte carte = popStack(stack);
		if (carte.id == id)
		{
			rezultat = carte;

			rezultat.autor = (char*)malloc(strlen(carte.autor) + 1);
			strcpy(rezultat.autor, carte.autor);
			rezultat.titlu = (char*)malloc(strlen(carte.titlu) + 1);
			strcpy(rezultat.titlu, carte.titlu);
		}
		else
			pushStack(&aux, carte);
	}
	//refacem
	while (!emptyStack(aux))
	{
		pushStack(stack, popStack(&aux));
	}
	return rezultat;
}
float calculeazaPretTotal(Stack* stack)
{
	float suma = 0;
	Stack aux;
	aux.end = NULL;
	aux.start = NULL;
	while (stack->end)
	{
		Carte carte = popStack(stack);
		suma += carte.pret;
		pushStack(&aux, carte);
	}
	//refacem
	while (!emptyStack(aux))
	{
		pushStack(stack, popStack(&aux));
	}
	return suma;
}
int size(Stack* stack)
{
	int contor = 0;
	Stack aux;
	aux.end = NULL;
	aux.start = NULL;
	while (stack->end)
	{
		Carte carte = popStack(stack);
		contor++;
		pushStack(&aux, carte);
	}
	//refacem
	while (!emptyStack(aux))
	{
		pushStack(stack, popStack(&aux));
	}
	return contor;
}
int main()
{
	Stack stack = citireStivaDinFisier("carti.txt");
	Nod* p = stack.start;

	printf("Marimea stivei este:%d", size(&stack));

	Carte carte = getCarteByIDStack(&stack, 3);
	if (carte.id != -1)
	{
		printf("\nCartea este:\n");
		afisareCarte(carte);
		free(carte.autor);
		free(carte.titlu);
	}
	else printf("\nNu exista aceasta carte\n");


	//while (p)
	//{
	//	afisareCarte(p->info);
	//	p = p->next;
	//}

	float suma = calculeazaPretTotal(&stack);
	printf("\nPretul total al cartilor este: %.2f", suma);
	dezalocare(&stack);
	return 0;
}