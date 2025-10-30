#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int nb_sig = 1;
bool affichage = true;

void gestionnaire(int numSig) {

	switch (numSig) {

		case 2:

			nb_sig+=1;
			break;

		case 3:

			affichage = false;
			break;

	}

}

int main() {

	struct sigaction action;
	action.sa_handler = gestionnaire;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	sigaction(2, &action, NULL);

	sigaction(3, &action, NULL);

	struct sigaction ignore;
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;

	sigaction(20, &ignore, NULL);

	while (affichage) {

		sleep(1);
		printf("Nb de signaux reçus : %d\n", nb_sig);

	}

	printf("Programme terminé.\n");
	return 0;
}