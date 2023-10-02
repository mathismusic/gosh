#include "constants.h"
#include <string.h>
#include <stdio.h>

// tokenize string `line` by delimiters `delims`. Returns an array of null-terminated strings.
char **tokenize(char *line, char *delims) {
	char **tokens = (char **)malloc((MAX_NUM_TOKENS+1) * sizeof(char *));
    memset(tokens, 0, (MAX_NUM_TOKENS+1) * sizeof(char *)); // initialize tokens to NULL
	char token[MAX_TOKEN_SIZE];

    int inTokenIdx = 0, tokenIdx = 0;
    int n = strlen(line);
    // printf("n = %d\n", n);
	
    for (char *c = line; c < line + n && tokenIdx < MAX_NUM_TOKENS; c++) {
        // printf("token = %s\n", token);
		char ch = *c;
        if (strchr(delims, ch)) {
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
        } else {
            token[inTokenIdx++] = ch;
        }
	}
	tokens[tokenIdx] = NULL;
	return tokens;
}