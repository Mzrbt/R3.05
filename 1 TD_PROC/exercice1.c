#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int pid;
    pid = fork();

    if (pid == -1) {

        perror("fork");
        exit(EXIT_FAILURE);

    }

    if (pid == 0) {

        printf("Je suis le fils, mon PID est %d et mon père a le PID %d\n", getpid(), getppid());

    } else {

        wait(NULL); //Nécessaire pour ne pas que le proc père meurt avant le fils et qu'il se fasse adopter par un proc système 
        printf("Je suis le père, mon PID est %d et j'ai crée un fils dont le PID est %d\n", getpid(), pid);

    }

}