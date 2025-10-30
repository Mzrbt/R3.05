#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {

	pid_t pid = fork();

	if (pid == 0) {

		while(1) {

			sleep(1);

		}
		exit(0);
	}

	if (kill(pid, 0) == 0) {

		printf("Fils crée\n");

	}

	sleep(2);

	kill(pid, SIGUSR1);

	sleep(2);

	int status;
	waitpid(pid, &status, 0);

	if (WIFSIGNALED(status)) {

		printf("fils tué par le signal %d\n", WTERMSIG(status));

	} else if (WIFEXITED(status)) {

		printf("Fils terminé normalement avec le code %d\n", WEXITSTATUS(status));

	}

}