#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

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
#define RESET_COLOR 7
#define CWD_COLOR 2
#define PROMPT_COLOR 1

#define NULL 0
#endif