#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

typedef struct {

	double Vo;
	double Vc;

} Arguments;	

void *ThreadCalculs(void* arg);

int main(int argc, char* argv[]) {

	Arguments arguments;

	arguments.Vo = -2.3;
	arguments.Vc = 1.7;

	pthread_t th;

	double tab[4100];

	for (int i = 0; i < 4096; i++) {
		tab[i] = arguments.Vo + arguments.Vc * sin (2 * M_PI * i / 4096);
	}

	if ((pthread_create(&th, NULL, ThreadCalculs, (void *)tab)) != 0) {
		printf("Erreur dans la création du thread\n");
		return EXIT_FAILURE;
	}
	
	pthread_join (th, NULL);
	printf("Fin du thread calculs\n");

	printf("\n");

	printf("Valeurs calculées par le thread :\n");
	printf("Minimum : %f\n", tab[4096]);
	printf("Maximum : %f\n", tab[4097]);
	printf("Moyenne : %f\n", tab[4098]);
	printf("Variance : %f\n", tab[4099]);

}

void *ThreadCalculs(void* arg) {

	double* tab = (double*)arg;
	double max = tab[0];
	double min = tab[0];
	double moy = 0;
	double var = 0;

	for (int i = 0; i < 4096; i++) {

		if (tab[i] > max) max = tab[i];
		if (tab[i] < min) min = tab[i];

		moy += tab[i];

	}

	moy /= (double)4096;

	for (int i = 0; i < 4096; i++) {

		var += pow((tab[i] - moy), 2);

	}

	var *= 1/(double)4096;

	tab[4096] = min;
	tab[4097] = max;
	tab[4098] = moy;
	tab[4099] = var;

	return NULL;

}