#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void gestionnaire(int numSig) {

	switch (numSig) {

		case 10:

			printf("Père : Signal 10 reçu\n");
			break;

		case 12:

			printf("Père : Signal 12 reçu\n");
			break;
	}

}

int main() {

	struct sigaction action;
	action.sa_handler = gestionnaire;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	sigaction(10, &action, NULL);

	sigaction(12, &action, NULL);


	pid_t pid1 = fork();
	if (pid1 == 0) {

		kill(getppid(), SIGUSR1);

		exit(0);

	}

	pid_t pid2 = fork();
	if (pid2 == 0) {

		kill(getppid(), SIGUSR2);

		exit(0);

	}

	pid_t pid3 = fork();
	if (pid3 == 0) {

		sleep(3);

		exit(0);

	}

	waitpid(pid3, NULL, 0);

}