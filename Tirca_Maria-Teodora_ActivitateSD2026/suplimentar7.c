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

Student citireStudentDinFisier(FILE* file)
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

// ===STACK===

//LSI
struct Nod {
	Student info;
	struct Nod* next;
};
typedef struct Nod Nod;

void pushStack(Nod** stack, Student student)
{
	//adaugam la inceput

	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = student;
	nod->next = *stack;
	*stack = nod;
}

Student popStack(Nod** stack)
{
	//returneaza si sterge de la inceput

	Student student;
	student.id = -1;

	if (*stack != NULL)
	{
		Nod* nod = (*stack);
		*stack = nod->next;

		student = nod->info;

		//stergerea nodului. valorile alocate dinamic sunt pe heap neatinse deci trb sa le dam free in main
		free(nod);
	}
	return student;
}

char emptyStack(Nod* stack)
{
	return stack == NULL;
}
Nod* citireStackStudentiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	if (file)
	{
		Nod* stack = NULL;
		while (!feof(file))
		{
			Student student = citireStudentDinFisier(file);
			pushStack(&stack, student);
		}
		fclose(file);
		return stack;
	}
	return NULL;
}
void dezalocareStivaDeStudenti(Nod** stack)
{
	/*while (*stack != NULL)
	{
		Nod* aux = *stack;
		*stack = aux->next;

		if (aux->info.nume != NULL)
			free(aux->info.nume);
		if (aux->info.specializare != NULL)
			free(aux->info.specializare);

		free(aux);
	}*/

	while (*stack != NULL)
	{
		Student student = popStack(stack);

		//trb sa dau free si la campurile alocate dinamic
		if (student.nume != NULL)
		{
			free(student.nume);
		}
		if (student.specializare != NULL)
		{
			free(student.specializare);
		}
	}
}
int size(Nod** stack)
{
	Nod* aux = NULL;
	int contor = 0;

	while (*stack != NULL)
	{
		Student student = popStack(stack);
		contor++;
		pushStack(&aux, student);
	}

	//refacem stiva originala
	while (aux != NULL)
	{
		Student student = popStack(&aux);
		pushStack(stack, student);
	}

	return contor;
}
//metode de procesare
Student getStudentByIDStack(Nod** stack, int id)
{
	Nod* aux = NULL;
	Student rezultat;
	rezultat.id = -1;

	while (*stack != NULL)
	{
		Student student = popStack(stack);

		if (student.id == id)
		{
			rezultat = student;

			//deep copy pentru campurile alocate dinamic
			rezultat.nume = (char*)malloc(sizeof(char) * (strlen(student.nume) + 1));
			strcpy(rezultat.nume, student.nume);

			rezultat.specializare = (char*)malloc(sizeof(char) * (strlen(student.specializare) + 1));
			strcpy(rezultat.specializare, student.specializare);
		}

		pushStack(&aux, student);
	}

	//refacem stiva originala
	while (aux != NULL)
	{
		pushStack(stack, popStack(&aux));
	}

	return rezultat;
}

//===QUEUE===

//LDI
struct NodDublu
{
	Student info;
	struct NodDublu* prev;
	struct NodDublu* next;
};
typedef struct NodDublu NodDublu;

struct Queue
{
	NodDublu* start;
	NodDublu* end;
};
typedef struct Queue Queue;

void enqueue(Queue* queue, Student student)
{
	//adaugam la inceput

	NodDublu* nod = (NodDublu*)malloc(sizeof(NodDublu));
	nod->info = student;
	nod->next = queue->start;
	nod->prev = NULL;

	if (queue->start != NULL) //coada nu e goala
	{
		queue->start->prev = nod;
	}
	else
	{
		queue->end = nod;
	}

	queue->start = nod;
}

Student dequeue(Queue* queue)
{
	//extragem si stergem de la sfarsit

	Student student;
	student.id = -1;

	if (queue->end == NULL)
	{
		return student;
	}

	student = queue->end->info;

	if (queue->start == queue->end) //este doar un nod
	{
		free(queue->start);

		queue->start = NULL;
		queue->end = NULL;
	}
	else
	{
		queue->end = queue->end->prev;
		free(queue->end->next);
		queue->end->next = NULL;
	}

	return student;
}

Queue citireCoadaDeStudentiDinFisier(const char* numeFisier)
{
	Queue queue;
	queue.start = NULL;
	queue.end = NULL;

	FILE* file = fopen(numeFisier, "r");
	if (file)
	{
		while (!feof(file))
		{
			Student student = citireStudentDinFisier(file);
			enqueue(&queue, student);
		}

		fclose(file);
	}
	return queue;
}

void dezalocareCoadaDeStudenti(Queue* queue)
{
	/*NodDublu* nod = queue->start;
	while (nod != NULL)
	{
		NodDublu* aux = nod;
		nod = nod->next;

		if (aux->info.nume != NULL)
		{
			free(aux->info.nume);
		}
		if (aux->info.specializare != NULL)
		{
			free(aux->info.specializare);
		}

		free(aux);
	}

	queue->start = NULL;
	queue->end = NULL;*/

	while (queue->start != NULL)
	{
		Student student = dequeue(queue);

		//dezalocam si campurile dinamice
		if (student.nume != NULL)
		{
			free(student.nume);
		}
		if (student.specializare != NULL)
		{
			free(student.specializare);
		}
	}
}
Student getStudentByIDQueue(Queue* queue, int id)
{
	Student rezultat;
	rezultat.id = -1;
	rezultat.nume = NULL;
	rezultat.specializare = NULL;

	Queue aux;
	aux.start = NULL;
	aux.end = NULL;

	while (queue->start != NULL)
	{
		Student student = dequeue(queue);

		if (student.id == id)
		{
			rezultat = student;

			rezultat.nume = (char*)malloc(sizeof(char) * (strlen(student.nume) + 1));
			strcpy(rezultat.nume, student.nume);

			rezultat.specializare = (char*)malloc(sizeof(char) * (strlen(student.specializare) + 1));
			strcpy(rezultat.specializare, student.specializare);
		}

		enqueue(&aux, student);
	}

	while (aux.start != NULL)
	{
		enqueue(queue, dequeue(&aux));
	}

	return rezultat;
}
float calculeazaMediaGeneralaStack(Nod** stack)
{
	float suma = 0;
	Nod* aux = NULL;
	int nrStudenti = size(stack);

	while (!emptyStack(*stack))
	{
		Student student = popStack(stack);
		suma += student.medie;
		pushStack(&aux, student);
	}

	//acum sunt puse invers => o intoarcem
	while (!emptyStack(aux))
	{
		pushStack(stack, popStack(&aux));
	}

	if (nrStudenti > 0)
	{
		return suma / nrStudenti;
	}

	return 0;
}

int main()
{
	Nod* stack = citireStackStudentiDinFisier("studenti.txt");
	printf("S-au citit %d studenti in stiva.\n\n", size(&stack));

	printf("\nCautam studentul cu ID 2 in stiva\n");
	Student sCautat = getStudentByIDStack(&stack, 2);
	if (sCautat.id != -1)
	{
		afisareStudent(sCautat);
		free(sCautat.nume);
		free(sCautat.specializare);
	}

	//afisareStudent(popStack(&stack));
	//afisareStudent(popStack(&stack));

	float medie = calculeazaMediaGeneralaStack(&stack);
	printf("Media generala a studentilor din stiva este %.2f\n\n", medie);

	dezalocareStivaDeStudenti(&stack);

	Queue queue;
	queue.end = NULL;
	queue.start = NULL;
	queue = citireCoadaDeStudentiDinFisier("studenti.txt");

	//afisareStudent(dequeue(&queue));
	//afisareStudent(dequeue(&queue));

	Student student = getStudentByIDQueue(&queue, 9);
	if (student.id != -1)
	{
		afisareStudent(student);
		free(student.nume);
		free(student.specializare);
	}
	else
	{
		printf("Studentul cu ID 9 nu a fost gasit.\n");
	}
	dezalocareCoadaDeStudenti(&queue);

	return 0;
}