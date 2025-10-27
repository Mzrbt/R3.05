#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    printf("Résultat de la commande ps fl :\n");
    system("ps fl");
    printf("Fin\n");

    printf("Résultat de la commande ps fl :\n");
    execlp("ps", "ps", "fl", NULL);//Remplace à partir d'ici le processus par la commande ce qui fait que la dernière ligne ne s'affiche pas
    printf("Fin\n");

    return 0;

}