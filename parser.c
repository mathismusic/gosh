#include "constants.h"
#include <stdio.h>

enum cmd_type {
    LIST,
    PIPE,
    REDIRECT,
};

typedef struct list_cmd {
    struct list_cmd *next;
} list_cmd;

typedef struct pipe_cmd {
    int pfd[2];
    struct pipe_cmd *next;
} pipe_cmd;

typedef struct redirect_cmd {
    char *file;
    struct redirect_cmd *next;
} redirect_cmd;

typedef struct cmd {
    int type;
    char *cmd;
    char *argv[MAX_NUM_TOKENS];
    struct list_cmd *lcmd; // only one of these will be non-null
    struct pipe_cmd *pcmd;
    struct redirect_cmd *rcmd;
} cmd;

const cmd empty_cmd = {0, 0, 0, 0, 0, 0};

cmd* parse(char *line) {
    // tree parsing, time = O(n)
    // first parse by pipe
    cmd *root = malloc(sizeof(cmd));
    *root = empty_cmd;
    // tokenize manually
    char **tokens = (char **)malloc((MAX_NUM_TOKENS+1) * sizeof(char *));
    memset(tokens, 0, (MAX_NUM_TOKENS+1) * sizeof(char *)); // initialize tokens to NULL
	char token[MAX_TOKEN_SIZE];

    int inTokenIdx = 0, tokenIdx = 0;
    int n = strlen(line);
    // printf("n = %d\n", n);
	
    for (char *c = line; c < line + n && tokenIdx < MAX_NUM_TOKENS; c++) {
        // printf("token = %s\n", token);
		char ch = *c;
        if (strchr(whitespace, ch)) {
            if (inTokenIdx == 0) continue; // skip consecutive delimiters

            token[inTokenIdx] = '\0'; // null terminate the token
            if (strlen(token) >= MAX_TOKEN_SIZE) {
                fprintf(stderr, "Token size exceeded. Exiting.\n");
                free(tokens);
                exit(1);
            }
            tokens[tokenIdx] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
            strcpy(tokens[tokenIdx], token);
            inTokenIdx = 0;
            tokenIdx++;
        } else if (ch == '|') {
            // printf("Found a pipe\n");
            
        }
	}
	tokens[tokenIdx] = NULL;
	return tokens;
    
}