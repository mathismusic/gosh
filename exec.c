#include <stdio.h>
#include "constants.h"

int run(char *cmd, char **args, int fg){
	int rc = fork(); if (rc < 0) {
		fprintf(stderr, "fork failed, could not execut command.\n"); return 1;
	} if (rc == 0) {
		execvp(cmd, args);
		fprintf(stderr, "execvp failed, could not execute command %s%s%s.\n", pallete[0], cmd, pallete[7]);
		exit(1); // if execvp returns, it is an error
	} else if (fg == 1) wait(0); // shell waits only for foreground processes
	return 0;
}