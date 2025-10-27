#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int fils(int valeur) {

    int max = 1000 * valeur;
    
    for (int i = 0; i < max; i++) {

    }

    printf("\tFils N° %d : fin max = 0x%x\n", valeur, max);
    return max;
}

int main() {

    const int NB_FILS = 3;
    pid_t pids[NB_FILS];
    int status;

    printf("Père (PID %d) : création de %d fils\n", getpid(), NB_FILS);

    for (int i = 0; i < NB_FILS; i++) {

        pid_t pid = fork();

        if (pid == -1) {

            perror("fork");
            exit(EXIT_FAILURE);

        }

        if (pid == 0) {

            int val_retour = fils(i + 1);
            exit(val_retour % 256);

        }
        pids[i] = pid;
    }

    for (int i = 0; i < NB_FILS; i++) {

        pid_t ended = waitpid(pids[i], &status, 0);

        if (ended == -1) {

            perror("waitpid");
            continue;

        } else {

            int code = WEXITSTATUS(status);
            printf("Père : le fils PID=%d s'est terminé normalement avec le code %d\n", ended, code);

        }
    }

    printf("Père : tous les fils sont terminés.\n");
    return 0;

}