#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

	int pipefd[2];
	char buf;

	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();

	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {

		close(pipefd[1]);

		while (read(pipefd[0], &buf, 1) > 0) {

			if (isalpha(buf)) {

				write(STDOUT_FILENO, &buf, 1);

			}
		}

		close(pipefd[0]);

		exit(EXIT_SUCCESS);

	}

	close(pipefd[0]);

	while (read(STDIN_FILENO, &buf, 1) > 0) {

		write(pipefd[1], &buf, 1);

	}

	close(pipefd[1]);

	wait(NULL);
}