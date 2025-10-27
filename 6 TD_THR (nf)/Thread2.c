#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

typedef struct {

	double tab[4100];
	pthread_mutex_t* mutex;

} Arguments;	

void *ThreadCalculs(void* arg);

int main(int argc, char* argv[]) {

	Arguments arguments;

	double Vo = -2.3;
	double Vc = 1.7;

	pthread_t th;
	pthread_mutex_t mutex;

	if ((pthread_mutex_init(&mutex, NULL)) != 0) {
		printf("Erreur d'initialisation du mutex");
		return EXIT_FAILURE;
	}

	if ((pthread_mutex_lock(&mutex)) != 0) {
		printf("Erreur de verouillage du mutex");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < 4096; i++) {
		arguments.tab[i] = Vo + Vc * sin (2 * M_PI * i / 4096);
	}

	if ((pthread_create(&th, NULL, ThreadCalculs, (void *)&arguments)) != 0) {
		printf("Erreur dans la création du thread\n");
		return EXIT_FAILURE;
	}
	
	pthread_join (th, NULL);
	printf("Fin du thread calculs\n");

	printf("\n");

	printf("Valeurs calculées par le thread :\n");
	printf("Minimum : %f\n", arguments.tab[4096]);
	printf("Maximum : %f\n", arguments.tab[4097]);
	printf("Moyenne : %f\n", arguments.tab[4098]);
	printf("Variance : %f\n", arguments.tab[4099]);

}

void *ThreadCalculs(void* arg) {

	Arguments* args = (Arguments*)arg;
	double* tab = args->tab;
	pthread_mutex_t* mutex = args->mutex;

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