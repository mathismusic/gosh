#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "constants.h"
#include "tok.c"
#include "exec.c"

int main(int argc, char *argv[])
{
	char line[MAX_INPUT_SIZE];
	char **tokens;
	int num_tokens;

	// current working directory for the prompt
    char cwd[MAX_TOKEN_SIZE];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return 1;
    }

	while (1) {	
		// reap all zombie children (processes in the background that have finished)
		while (waitpid(-1, NULL, WNOHANG) > 0)
			;
		
		// take input from the shell stdin
        memset(line, 0, sizeof(line));
		printf("%s↪ %s %s %s$ %s", pallete[getpid()%7], pallete[CWD_COLOR], cwd, pallete[PROMPT_COLOR], pallete[RESET_COLOR]);
		fflush(stdout);
		scanf("%[^\n]%*c", line);
		// printf("line = ;%s;\n", line);

		// printf("Command entered: '%s' (REMOVE THIS OUTPUT LATER)\n", line);

		line[strlen(line)] = '\n'; // terminate with new line (makes sure that the last token is parsed properly in tok.c)
		line[strlen(line)+1] = '\0';

		// first parse by pipe
		tokens = tokenize(line, whitespace);

		if (tokens[0] == NULL)	// if empty command, ask for as command again in new line
            continue;

		// compute the number of tokens
		num_tokens = 0;
		while(tokens[num_tokens++]);
		num_tokens--;

		// Printing each found token in a new line for debugging
		// for (int i = 0; tokens[i] != NULL; i++)
		// {
		// 	// printf("Found the #%d token: '%s' (REMOVE THIS OUTPUT LATER)\n", i, tokens[i]);
		// }

		// some default commands
		if (tokens[1] == NULL) {
			// exiting the shell on exit
			if (strcmp(tokens[0], "exit") == 0 || strcmp(tokens[0], "q") == 0) {
				printf("Exiting shell...\n");
				exit(0);
			} else if (strcmp(tokens[0], "gosh") == 0) {
				free(tokens[0]);
				tokens[0] = "./main";
				run(tokens[0], tokens, 1);
				continue;
			}
		}

		// change directory
		if (strcmp(tokens[0], "cd") == 0) {
			if (tokens[2] != NULL) {
				printf("cd: too many arguments\n");
				continue;
			}
			if (tokens[1] == NULL || strcmp(tokens[1], "~") == 0) {
				strcpy(cwd, getenv("HOME"));
				chdir(cwd);
			} else if (strcmp(tokens[1], "-") == 0) {
				strcpy(cwd, getenv("OLDPWD"));
				chdir(cwd);
			} 
			else {
				// concat cwd and tokens[1] into cwd
				if (chdir(tokens[1])) {
					printf("cd: no such file or directory: %s\n", tokens[1]);
				} else if (getcwd(cwd, sizeof(cwd)) == NULL) {
					perror("getcwd() error");
					return 1;
				}
			}
			continue;
		}
		
		// new command to kill a process running in the background
		if (num_tokens == 2 && strcmp(tokens[0], "end") == 0) {
			int pid = atoi(tokens[1]);
			if (kill(pid, SIGTERM)) {
				printf("kill: no such process\n");
			}
			continue;
		}

		for (int i = 0; tokens[i] != NULL; i++)
		{
			printf("Found the #%d token: '%s' (REMOVE THIS OUTPUT LATER)\n", i, tokens[i]);
		}

		// otherwise general command that the child can run
		
		// check if the command is to be run in the background
		if (num_tokens > 1 && strcmp(tokens[num_tokens-1], "&") == 0) {
			tokens[num_tokens-1] = NULL;
			run(tokens[0], tokens, 0);
		}
		else run(tokens[0], tokens, 1);

		// Freeing the allocated memory to the tokens
		for (int i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}