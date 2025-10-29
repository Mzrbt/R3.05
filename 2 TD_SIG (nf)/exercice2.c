#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int sig1_recu = 0;

void gestionnaire(int numSig) {

	switch (numSig) {

		case 10:

			if (sig1_recu == 0) {

				printf("Bonjour, Signal SIGSUR1\n");
				sig1_recu = 1;

			} break;
			
		case 12:

			printf("Bonsoir, Signal SIGSUR2\n");	
			break;

	}
}

int main() {

	struct sigaction action;

	printf("Je suis le PID = %d\n", getpid());

	action.sa_handler = gestionnaire;
	sigemptyset(&action.sa_mask);
   	action.sa_flags = 0;

   	sigaction(10, &action, NULL);

   	sigaction(12, &action, NULL);

   	while(1) {

   		pause();

   	}
}