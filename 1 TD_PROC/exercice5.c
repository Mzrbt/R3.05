#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pp, ps;
    int status;
    printf("Je suis le processus administrateur de PID %d\n", getpid());
    pp=fork();
    switch(pp) {
        case -1: printf("echec du fork\n"); exit(-1);
        case 0: printf("Je suis le processus fils de PID %d qui exécute la tâche principale\n",
            getpid());
        sleep(2);//exécution de la tâche principale
        printf("Fin de la tâche principale\n");
        exit(1);
    default: ps=fork();
        switch(ps) {
            case -1: printf("echec de fork"); exit(-2);
            case 0: printf("Je suis le processus fils de PID %d qui exécute la tâche secondaire\n", getpid());
            sleep(10);// exécution de la tâche secondaire
            printf("Fin de la tâche secondaire\n");
            exit(2);
            default: printf("Je suis le processus administrateur, j’attends la fin de soit l'une des deux tâches, la principale, la secondaire et les deux\n");

            wait(&status);

            waitpid(pp, &status, 0);

            waitpid(ps, &status, 0);

            waitpid(pp, &status, 0);
            waitpid(ps, &status, 0);

            printf("Fin du processus administrateur\n");
        }
    }
    return(0);
}