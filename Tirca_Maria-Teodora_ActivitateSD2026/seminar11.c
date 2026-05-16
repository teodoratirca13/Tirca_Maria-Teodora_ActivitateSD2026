//graful este o multime de noduri  si de muchii care vor lega aceste noduri

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodPrincipal {
	Masina info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

struct NodSecundar {
	NodSecundar* next;
	NodPrincipal* info;
};

void inserareListaPrincipala(NodPrincipal** graf, Masina masinaNoua) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->vecini = NULL;
	if ((*graf) != NULL) {
		NodPrincipal* aux = *graf;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		(*graf) = nou;
	}
}

void inserereListaSecundata(NodSecundar** cap, NodPrincipal* nod) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->info = nod;
	nou->next = NULL;
	if ((*cap) != NULL) {
		NodSecundar* aux = (*cap);
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		(*cap) = nou;
	}
}

NodPrincipal* cautaNodDupaID(NodPrincipal* listaPrincipala, int idCautat) {
	while (listaPrincipala->next != NULL && listaPrincipala->info.id != idCautat) {
		listaPrincipala = listaPrincipala->next;
	}
	return listaPrincipala;
}

void inserareMuchie(NodPrincipal* listaPrincipala, int idStart, int idStop) {
	if (listaPrincipala) {
		NodPrincipal* start = cautaNodDupaID(listaPrincipala, idStart);
		NodPrincipal* stop = cautaNodDupaID(listaPrincipala, idStop);
		if (start && stop) {
			inserereListaSecundata(&(start)->vecini, stop);
			inserereListaSecundata(&(stop)->vecini, start);
		}
	}
}


NodPrincipal* citireNoduriMasiniDinFisier(const char* numeFisier) {
	
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* graf = NULL;
	if(f)
	{
		while (!feof(f)) {
			inserareListaPrincipala(&graf, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return graf;	
}

void citireMuchiiDinFisier(const char* numeFisier, NodPrincipal* listaP) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			int start, stop; //sunt id uri
			fscanf(f, "%d %d", &start, &stop);
			inserareMuchie(listaP, start, stop);
		}
	}
	fclose(f);
}

void dezalocareNoduriGraf(void* listaPrincipala) {
	
}
void afisareLista(NodPrincipal* graf, int id)
{
	NodPrincipal* nod = cautaNodDupaID(graf, id);
	NodSecundar* cap = nod->vecini;
	while (cap) {
		afisareMasina(cap->info->info);
			cap = cap->next;
	}
}

int main() {
	NodPrincipal* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt",graf);
	afisareLista(graf, 1);
	return 0;
}