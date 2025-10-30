#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

	int pipefd[2];
	int pipefd2[2];
	char buf;

	int compteur = 0;

	if (pipe(pipefd) == -1) {
		perror("pipe 1");
		exit(EXIT_FAILURE);
	}

	if (pipe(pipefd2) == -1) {
		perror("pipe 2");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();

	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {

		close(pipefd[1]);
		close(pipefd2[0]);

		while (read(pipefd[0], &buf, 1) > 0) {

			if (isalpha(buf)) {

				buf = toupper(buf);
				write(STDOUT_FILENO, &buf, 1);
				compteur++;

			}
		}

		close(pipefd[0]);

		write(pipefd2[1], &compteur, sizeof(compteur));

		close(pipefd2[1]);

		exit(EXIT_SUCCESS);

	}

	close(pipefd[0]);
	close(pipefd2[1]);

	while (read(STDIN_FILENO, &buf, 1) > 0) {

		write(pipefd[1], &buf, 1);

	}

	close(pipefd[1]);

	read(pipefd2[0], &compteur, sizeof(compteur));
	printf("Nombre de caractères alphabétiques : %d\n", compteur);

	close(pipefd2[0]);

	wait(NULL);
}