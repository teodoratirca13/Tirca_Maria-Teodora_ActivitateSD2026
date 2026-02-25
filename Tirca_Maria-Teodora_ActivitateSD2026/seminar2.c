#include<stdio.h>
#include<malloc.h>

struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret ;
	char  serie;
};
struct Telefon initializare(int id, int RAM, char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.pret = pret;
	t.serie = serie;
	t.producator = (char*)malloc(strlen(producator) + 1);
	strcpy(t.producator, producator);
	return t;
}

void afisare(struct Telefon t) {
	printf("Id: %d\n", t.id);
	printf("RAM: %d\n", t.RAM);
	printf("Producator: %s\n", t.producator);
	printf("Pret: %5.2f\n", t.pret);
	printf("Serie: %c\n", t.serie);
	printf("\n", t.id);
}

void afisareVector(struct Telefon* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++)
		afisare(vector[i]);
}
struct Telefon copiazaTelefon(struct Telefon t) //pt deep copy
{
	struct Telefon tel = initializare(t.id, t.RAM, t.producator, t.pret, t.serie);
	return tel;
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	struct Telefon *vectorNou=NULL;
	if (nrElementeCopiate > nrElemente)
		nrElementeCopiate = nrElemente;
	vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++)
	{
		vectorNou[i]=copiazaTelefon(vector[i]);
	}
	return vectorNou;
	
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++)
	{
		free((*vector)[i].producator);
		(*vector)[i].producator = NULL;
	}
	free(*vector); //pt a nu avea memory leak
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pretMinim, struct Telefon** vectorNou, int* dimensiune) {

	*dimensiune=0;
	for (int i = 0; i < nrElemente; i++)
	{
		if(vector[i].pret >= pretMinim)
			(*dimensiune)++;
	}
	//vectorNou = (struct Telefon**)malloc(sizeof(struct Telefon) * (*dimensiune));
    *vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * (*dimensiune));

	int j = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret >= pretMinim)
		{
			(*vectorNou)[j] = copiazaTelefon(vector[i]);
			j++;
		}
	}
}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon s;
	s.id = 1;

	return s;
}
	


int main() {

	struct Telefon t;
	t = initializare(1, 100, "Samsung", 2500.0, 'A');
	
	struct Telefon* telefoane;
	int nrTelefoane=3;
	telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0] = t;
	telefoane[1] = initializare(2, 200, "Huawei", 3500.0, 'S');
	telefoane[2] = initializare(3, 250, "Iphone", 4000.0, 'C');
	afisareVector(telefoane, nrTelefoane);
	
	int nrTelefoaneCopiate = 2;
	struct Telefon* rezultat = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrTelefoaneCopiate);

	printf("Copiere primele n telefoane:\n\n");
	afisareVector(rezultat,nrTelefoaneCopiate);

	struct Telefon* telScumpe;
	int dim = 0;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 3000, &telScumpe, &dim);
	printf("Telefonae scumpe:\n\n");
	afisareVector(telScumpe, dim);

	dezalocare(&telefoane, &nrTelefoane);
	dezalocare(&rezultat,&nrTelefoaneCopiate);

}