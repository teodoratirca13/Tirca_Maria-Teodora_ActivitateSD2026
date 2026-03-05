#include<stdio.h>
#include<stdlib.h>

struct Produs {
	int id;
	int stoc;
	char* denumire;
	float pret;
	char categorie;
};

struct Produs initializare(int id, int stoc, char* denumire, float pret, char categorie) {
	struct Produs s;
	s.id = id;
	s.stoc = stoc;
	s.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(s.denumire, denumire);
	s.pret = pret;
	s.categorie = categorie;
	return s;
}


void afisare(struct Produs s) {
	printf("Id: %d\n", s.id);
	printf("Stoc: %d\n", s.stoc);
	printf("Denumire: %s\n", s.denumire);
	printf("Pret: %5.2f\n", s.pret);
	printf("Categorie: %c\n", s.categorie);
}

void modifica_Atribut(struct Produs* s, char* nouaDenumire) {

	free((*s).denumire);
	s->denumire = (char*)malloc(sizeof(char) * (strlen(nouaDenumire) + 1));
	strcpy(s->denumire, nouaDenumire);   
}

void dezalocare(struct Produs* s) {
	if (s->denumire != NULL)
	{
		free(s->denumire);
		s->denumire = NULL;
	}
}
float calculeazaTotal(struct Produs s)
{
	return s.stoc * s.pret;
}

int main() {
	struct Produs s;
	//char* c;
	//printf("Un pointer ocupa: %d", sizeof(c));

	s = initializare(1, 20, "Telefon smart", 2000, 'A');
	afisare(s);
	modifica_Atribut(&s, "Mouse");
	afisare(s);
	float valoare = calculeazaTotal(s);
	printf("Valoare totala pt %s este: %5.2f RON", s.denumire, valoare);

	struct Produs* ps = &s; //ps retine o adresa de pe stack
	struct Produs* pp = malloc(sizeof(struct Produs));

	free(pp);
	//free(ps); //eroare

	dezalocare(&s);
	return 0;
}