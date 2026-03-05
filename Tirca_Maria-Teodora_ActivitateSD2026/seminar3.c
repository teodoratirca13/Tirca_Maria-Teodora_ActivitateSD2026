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
typedef struct StructuraMasina Masina; //peste tot unde gaseste Masina, inlocuieste cu Masina

void afisareMasina(Masina masina) {
	printf("ID:%d\n", masina.id);
	printf("Numar usi:%d\n", masina.nrUsi);
	printf("Pret:%5.2f\n", masina.pret);
	printf("Model:%s\n", masina.model);
	printf("Nume sofer:%s\n", masina.numeSofer);
	printf("Serie:%c\n", masina.serie);
	printf("\n");
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	if(masini == NULL || nrMasini == 0)
	{
		printf("Vectorul de masini este gol!\n");
		return;
	}
	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	Masina* temp = (Masina*)malloc(sizeof(Masina)* ((*nrMasini)+1));
	for (int i = 0; i < *nrMasini; i++)
	{
		temp[i] = (*masini)[i];
	}
	temp[*nrMasini] = masinaNoua;
	//dezalocam blocul vechi de memorie
	free(*masini);
	*masini = temp;
	(*nrMasini)++;
	//temp se afla pe stiva functiei deci se sterge la finalul functiei, dar mergea sa zici temp=NULL;
}

Masina citireMasinaFisier(FILE* file) {
	char linie[256];
	fgets(linie, 255, file);
	char separator[3]=",\n";

	Masina masina;

	masina.id = atoi(strtok(linie, separator));
	masina.nrUsi = atoi(strtok(NULL, separator));
	masina.pret = atof(strtok(NULL, separator));

	char* temp1 = strtok(NULL, separator);
	masina.model = (char*)malloc(strlen(temp1) + 1);
	strcpy(masina.model, temp1);

	char* temp2 = strtok(NULL, separator);
	masina.numeSofer = (char*)malloc(strlen(temp2) + 1);
	strcpy(masina.numeSofer, temp2);

	masina.serie = strtok(NULL, separator)[0];

	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r");
	if (!file)
	{
		printf("Eroare la deschidere!\n");
		return;
	}
	else
	{
		Masina* vectorMasini = NULL;
		*nrMasiniCitite = 0;
		while (!feof(file))
		{
			Masina masina = citireMasinaFisier(file);
			adaugaMasinaInVector(&vectorMasini, nrMasiniCitite, masina);
		}
		return vectorMasini;
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	if (vector == NULL || *vector==NULL|| nrMasini==0|| *nrMasini == 0)
	{
		return;
	}
	for (int i = 0; i < *nrMasini; i++)
	{
		free((*vector)[i].model);
		(*vector)[i].model = NULL;
		free((*vector)[i].numeSofer);
		(*vector)[i].numeSofer = NULL;
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;
}

int main() {
	int nrMasini = 0;
	Masina* masini=citireVectorMasiniFisier("masini.txt",&nrMasini);

	afisareVectorMasini(masini,nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}