#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

	printf("Je suis le père, mon PID est %d\n", getpid());

    for (int i = 0; i < 3; i++){

        pid_t pid = fork();

        if (pid == -1) { 

        	perror("fork"); 
        	exit(EXIT_FAILURE); 

        }

        if (pid == 0) { 

            printf("Je suis le fils %d, mon PID est %d et mon père a le PID %d\n", i + 1, getpid(), getppid());
            sleep(i + 1);
            exit(EXIT_SUCCESS);
          
        }
    }

    for (int i = 0; i < 3; i++) {

        wait(NULL);

    }

    printf("Tout les fils sont terminés !\n");
    return 0;
}

//Le père est en charge uniquement de la création dans la première boucle car il ne rentre pas dans les deux if
//Les 3 fils sont donc crée et dorment pendant i + 1
//On éxécute ensuite la deuxième boucle qui indique au père d'attendre la fin de ses 3 fils
//On affiche ensuite que tout les fils sont terminés