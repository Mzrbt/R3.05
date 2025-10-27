#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

int creer_nSem(int n, int val){

	int semid;

	semid = semget(IPC_PRIVATE, n, IPC_CREAT | 0666);
	if (semid == -1){
		perror("Erreur de création de l'ensemble");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i++) {
		if (semctl(semid, i, SETVAL, val) == -1) {
			perror("Erreur d'initialisation sémaphore");
			exit(EXIT_FAILURE);
		}
	}

	return semid;

}

void afficheValeur(int semid, int semnum) {

	int val = semctl(semid, semnum, GETVAL);
	if (val == -1) {
		perror("Erreur GETVAL");
		exit(EXIT_FAILURE);
	}

	printf("La valeur du sémaphore %d est : %d\n", semnum, val);
	printf("\n");

}

void testFcts() {

	int semid = creer_nSem(6, 0);

	for (int i = 0; i < 6; i++) {
    	afficheValeur(semid, i);
	}

}

void Redacteur(int n, int *zdc) {

	for (int i = 0; i < n; i++) {
		zdc[i] = rand() % 101;
	}

}

int Minimum(int *zdc, int n) {

	int mini = zdc[0];

	for (int i = 1; i < n; i++) {
		if (zdc[i] < mini) {
			mini = zdc[i];
		}
	}

	return mini;

}

int Maximum(int *zdc, int n) {

	int maxi = zdc[0];

	for (int i = 1; i < n; i++){
		if (zdc[i] > maxi){
			maxi = zdc[i];
		}
	}

	return maxi;
}

float Moyenne(int *zdc, int n) {

	float moy = 0;

	for (int i = 0; i < n; i++){
		moy += zdc[i];
	}

	return moy/n;

}

void appSem() {

	int n = 10;

	int semid1 = creer_nSem(1, 0);
	int semid2 = creer_nSem(1, 0);

	int shmid1 = shmget(IPC_PRIVATE, (n + 2) * sizeof(int), IPC_CREAT | 0666);
	int shmid2 = shmget(IPC_PRIVATE, sizeof(float), IPC_CREAT | 0666);

	for (int i = 0; i < 4; i++) {

		pid_t pid = fork();

		if (pid == 0) {

			int *zdc1 = shmat(shmid1, NULL, 0);
			if (zdc1 == (void *)-1) {
				perror("Erreur d'attachement ZDC1");
				exit(EXIT_FAILURE);
			}

			float *zdc2 = shmat(shmid2, NULL, 0);
			if (zdc2 == (void *)-1){
				perror("Erreur d'attachement ZDC2");
				exit(EXIT_FAILURE);
			}

			switch(i) {

				case 0:

					Redacteur(n, zdc1);

					for (int j = 0; j < 3; j++) {
						struct sembuf op = {0, 1, 0};
                        semop(semid1, &op, 1);
					}
					break;

				case 1:
				{

					struct sembuf op = {0, -1, 0};
					semop(semid1, &op, 1);

					int min = Minimum(zdc1, n);
					zdc1[n] = min;

					struct sembuf op2 = {0, 1, 0};
					semop(semid2, &op2, 1);
					break;

				}

				case 2:
				{

					struct sembuf op = {0, -1, 0};
                    semop(semid1, &op, 1);

                    int max = Maximum(zdc1, n);
                    zdc1[n+1] = max;

                    struct sembuf op2 = {0, 1, 0};
                    semop(semid2, &op2, 1);
                    break;

				}

				case 3:
                {

                    struct sembuf op = {0, -1, 0};
                    semop(semid1, &op, 1);

                    float moy = Moyenne(zdc1, n);
                    *zdc2 = moy;

                    struct sembuf op2 = {0, 1, 0};
                    semop(semid2, &op2, 1);
                    break;

                }
			}

			exit(EXIT_SUCCESS);
		
		}
	}

	for (int i = 0; i < 3; i++) {
        struct sembuf op = {0, -1, 0};
        semop(semid2, &op, 1);
    }

    int *zdc1 = shmat(shmid1, NULL, 0);

	if (zdc1 == (void *)-1) { 
		perror("shmat zdc1"); 
		exit(EXIT_FAILURE); 
	}

    float *zdc2 = shmat(shmid2, NULL, 0);

    if (zdc2 == (void *)-1) { 
		perror("shmat zdc2"); 
		exit(EXIT_FAILURE); 
	}

    printf("Tableau généré :\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", zdc1[i]);
    }
    printf("\nMin : %d\n", zdc1[n]);
    printf("Max : %d\n", zdc1[n+1]);
    printf("Moyenne : %.2f\n", *zdc2);

    int pid1_mod = semctl(semid1, 0, GETPID);
	int pid2_mod = semctl(semid2, 0, GETPID);
	printf("Dernier proc ayant modifié semid1 : %d\n", pid1_mod);
	printf("Dernier proc ayant modifié semid2 : %d\n", pid2_mod);

	int pid1_blo = semctl(semid1, 0, GETNCNT);
	int pid2_blo = semctl(semid2, 0, GETNCNT);
	printf("Nombre de procs bloqués dans semid1 : %d\n", pid1_blo);
	printf("Nombre de procs bloqués dans semid2 : %d\n", pid2_blo);

    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    shmctl(shmid1, IPC_RMID, NULL);
	shmctl(shmid2, IPC_RMID, NULL);
	semctl(semid1, 0, IPC_RMID);
	semctl(semid2, 0, IPC_RMID);

}

int main (int argc, char *argv[]){

	srand(time(NULL));

	//testFcts();

	appSem();

    return 0;

}