/*
 *
 * author name(s), date, and other info here
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "list.h"

char** tokenify(const char *s) {
    int len = strlen(s);

    // token position and length flag
    int* token_flag = malloc((len + 1) * sizeof(int));
    int token_count = 0; // the total num of tokens
    int counter = 0;    // temp counter for token length

    // marks beginning of each token with token length
    for (int i = len - 1; i >= 0; i--) {
        // ignore and nullify any token after '#'
        if (s[i] == '#') {
            counter = 0;
            token_count = 0;
            for (int j = i; j < len; j++) { token_flag[j] = 0; }
            continue;
        }

        // count every non-space for each token length
        if (!isspace(s[i])) { counter++;}

        // create flags for beginning of each token, containing its length
        if (counter > 0 && (i == 0 || isspace(s[i-1]))) {
            token_flag[i] = counter;
            counter = 0;
            token_count++;
        }
        else {
            token_flag[i] = 0;
        }
    }

    // the tokens to return
    char** tokens = malloc((token_count + 1) * sizeof(char*));

    int j = 0;

    // assign tokens for each token pointer
    for (int i = 0; i < token_count; i++) {
        while (token_flag[j] < 1) { j++; }
        int token_len = token_flag[j];  // length of each token

        // allocate each token space
        tokens[i] = malloc((token_len + 1) * sizeof(char));

        for (int lcount = 0; lcount < token_len; lcount++) {
            tokens[i][lcount] = s[j];
            j++;
        }
        tokens[i][token_len] = '\0';
    }

    tokens[token_count] = NULL;

    free(token_flag);

    return tokens;
}


// check if the token is a proper int
bool proper_int(char *str) {
    int len = strlen(str);
    if (len <= 0) {
        return false;
    }
    else if (len == 1) {
        if (isdigit(str[0])) { return true; }
    }
    else {
        if (str[0] != '-' && (!isdigit(str[0]))) { return false; }
        for (int i = 1; i < len; i++) {
            if (!isdigit(str[i])) { return false;}
        }
        return true;
    }
}


void process_data(FILE *input_file) {
    char *content = malloc(1000 * sizeof(char));
    struct node *list = NULL;
    
    while (fgets(content, 1000, input_file) != NULL) {
        char **tokens = tokenify(content);
        char **token_curr = tokens;

        while (*token_curr != NULL) {
            if (proper_int(*token_curr)) {
                list_insert(atoi(*token_curr), &list);
            }
            free(*token_curr);
            token_curr++;
        }
        free(tokens);
    }

    list_sort(&list);
    list_print(list);

    struct rusage *stat = malloc(sizeof(struct rusage));
    int errcode = getrusage(RUSAGE_SELF, stat);
    if (errcode != 0) {
        perror(strerror(errcode));
        list_clear(list);
        free(content);
        free(stat);
        return;
    }

    printf("User time: %f\n", ((double) stat->ru_utime.tv_sec) + (double) stat->ru_utime.tv_usec / 1000000);
    printf("System time: %f\n", ((double) stat->ru_stime.tv_sec) + (double) stat->ru_stime.tv_usec / 1000000);

    list_clear(list);
    free(content);
    free(stat);
    // no need to fclose: done in main
}


void usage(char *program) {
    fprintf(stderr, "usage: %s [<datafile>]\n", program);
    exit(1);
}

#ifndef AUTOTEST
int main(int argc, char **argv) {
    FILE *datafile = NULL;

    /* find out how we got invoked and deal with it */
    switch (argc) {
        case 1:
            /* only one program argument (the program name) */ 
            /* just equate stdin with our datafile */
            datafile = stdin;        
            break;

        case 2:
            /* two arguments: program name and input file */
            /* open the file, assign to datafile */
            datafile = fopen(argv[1], "r");
            if (datafile == NULL) {
                printf("Unable to open file %s: %s\n", argv[1], strerror(errno));
                exit(-1);
            }
            break;

        default:
            /* more than two arguments?  throw hands up in resignation */
            usage(argv[0]);
    }

    /* 
     * you should be able to just read from datafile regardless 
     * whether it's stdin or a "real" file.
     */
    process_data(datafile);
    fclose(datafile);
    return 0;
}
#endif
