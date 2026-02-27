#include<stdio.h>
#include<malloc.h>

struct Film
{
	int id;
	int durata; 
	char* titlu;
	float rating;
	char categorie;
};

struct Film initializare(int id, int durata, char* titlu, float rating,char categorie)
{
	struct Film f;
	f.id = id;
	f.durata = durata;
	f.rating = rating;
	f.categorie = categorie;
	f.titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy(f.titlu, titlu);

	return f;
}
void afisare(struct Film f) {
	printf("Id: %d\n",f.id);
	printf("Durata: %d\n",f.durata);
	printf("Titlu: %s\n",f.titlu);
	printf("Rating:%5.2f\n",f.rating);
	printf("Categorie:%c\n",f.categorie);
	printf("\n");
}

void  afisareVectorFilme(struct Film* filme, int nrFilme)
{
	for (int i = 0; i < nrFilme; i++)
	{
		afisare(filme[i]);
	}
}
struct Film copiazaFilm(struct Film f)
{
	struct Film filmNou;
	filmNou = initializare(f.id, f.durata, f.titlu, f.rating, f.categorie);
	return filmNou;
}

void dezalocare(struct Film** filme, int* nrFilme)
{
	//dezalocam elementele
	for (int i = 0; i < *nrFilme; i++)
	{
		free((*filme)[i].titlu);
		(*filme)[i].titlu = NULL;
	}
	//dezalocam vectorul;
	free(*filme);
	*filme = NULL;
	*nrFilme = 0;
}
struct Film* getFilmRatingMaxim(struct Film* filme, int nrFilme, int *dim)
{
	float Max = filme[0].rating;
	*dim=1; //nr de filme cu rating max
	for (int i = 1; i < nrFilme; i++)
	{
		if (filme[i].rating > Max)
		{
			Max = filme[i].rating;
			*dim = 1;
		}
		else if (filme[i].rating == Max)
		{
			*dim++;
		}
	}
	struct Film* filmeMax = (struct Film*)malloc(sizeof(struct Film) * (*dim));
	int j = 0;
	for (int i = 0; i < nrFilme; i++)
	{
		if (filme[i].rating == Max)
		{
			filmeMax[j] = copiazaFilm(filme[i]);
			j++;
		}
	}
	return filmeMax;
}
struct Film* adaugareElement(struct Film* filme, int* nrFilme, struct Film filmDeAdaugat)
{
	int nrNou = (*nrFilme) + 1;
	struct Film* temp = (struct Film*)malloc(sizeof(struct Film) * nrNou);
	for (int i = 0; i < *nrFilme; i++)
	{
		temp[i] = copiazaFilm(filme[i]);
	}
	temp[*nrFilme] = copiazaFilm(filmDeAdaugat);

	dezalocare(&filme, nrFilme);

	*nrFilme = nrNou;
	return temp;
}
void copiazaFilmeDeDurataMinima(struct Film* filme, int nrFilme, int pragMinute,struct Film** vectorNou, int* dimensiune)
{
	*dimensiune = 0;
	for (int i = 0; i < nrFilme; i++)
	{
		if (filme[i].durata > pragMinute)
			(*dimensiune)++;
	}
	*vectorNou = (struct Film*)malloc(sizeof(struct Film) * (*dimensiune));

	int j = 0;
	for (int i = 0; i < nrFilme; i++)
	{
		if (filme[i].durata > pragMinute)
		{
			(*vectorNou)[j] = copiazaFilm(filme[i]);
			j++;
		}
	}
}
int main()
{
	int nrFilme = 3;
	struct Film* filme;
	filme = (struct Film*)malloc(sizeof(struct Film) * nrFilme);
	
	filme[0] = initializare(1, 120, "Inception", 8.8, 'A');
	filme[1] = initializare(2, 150, "Avatar", 7.9, 'S');
	filme[2] = initializare(3, 180, "Interstellar", 9.1, 'A');

	printf("\nLista filme:\n");
	afisareVectorFilme(filme,nrFilme);
	
	printf("\nLista filme cu rating maxim:\n");
	int dim;
	struct Film* rez = getFilmRatingMaxim(filme, nrFilme,&dim);
	afisareVectorFilme(rez, dim);


	printf("\nDupa adaugarea unui film:\n");

	struct Film filmDeAdaugat = initializare(4, 169, "Dune", 9.1, 'A');

	filme = adaugareElement(filme, &nrFilme, filmDeAdaugat);
	afisareVectorFilme(filme, nrFilme);

	free(filmDeAdaugat.titlu);
	filmDeAdaugat.titlu = NULL;

	float pragMinute = 120;
	struct Film* vector;
	int dimensiune = 0;
	copiazaFilmeDeDurataMinima(filme, nrFilme, pragMinute, &vector, &dimensiune);

	printf("\nFilme de durata minima:\n");
	afisareVectorFilme(vector, dimensiune);

	dezalocare(&filme, &nrFilme);
	dezalocare(&rez, &dim);
	dezalocare(&vector, &dimensiune);
	return 0;
}