#include <stdio.h> // scanf, fprintf
#include <stdlib.h>// exit
#include <unistd.h>// execv
#include <string.h>// strcmp
#include <sys/types.h>// waitpid

char *pallete[] = {
    "\033[1;31m", // red
    "\033[1;32m", // green
    "\033[1;33m", // yellow
    "\033[1;34m", // blue 
    "\033[1;35m", // magenta
    "\033[1;36m", // cyan
    "\033[1;37m", // white
    "\033[0m" // reset
};
// a program to write a terminal
int main(){
    while(1){
        char cmd[100];
        printf("%s↪ %s", pallete[getpid()%7], pallete[7]);
        scanf("%s", cmd);
        if (strcmp(cmd, "exit") == 0) break;
        char *ptr = cmd;
        if (cmd[0] == '.' && cmd[1] == '/'){
            ptr += 2;
        }
        // create a process with fork, run the required executable on this process with execv
        int cpid = fork(); // note that input-output is shared between parent and child
        
        if (cpid < 0){ 
            fprintf(stderr, "Fork failed.\n"); 
            exit(1); 
        }
        if (cpid == 0){
            int retcode = execv(ptr, NULL); 
            printf("terminal: command not found: %s\n", cmd);
            printf("retcode = %d\n", retcode);
            exit(1); // if we return from execv, it must have failed
        }
        else waitpid(cpid, NULL, 0); // just wait for finish and 
    } printf("This terminal had pid %d.\n", getpid());
}