#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "command.h"

struct ProcessControlBlock **PCBS;
int PCBS_len;

/* Read file */
int GetLine(char *filename, char *buffer, size_t bufferSize) {

    int file = open(filename, O_RDONLY);
    if (file == -1) {
        printf("Error: Unable to open file '%s'\n", filename);
        exit(1);
        return -1;
    }

    if (read(file, buffer, bufferSize) == -1) {
        printf("Error: Unable to read file '%s'\n", filename);
        return -1;
    }
    close(file);
    return 1;
}

/* Split line read from file into individual commands */
int ParseCommand(char **arr, size_t arrSize, struct ProcessControlBlock **PCBS) {

    const char *s = " ";

    /* The purpose of args is to store the arguments corrresponding to the 
       command that was given. This will be sent to makeCall_File along with
       the string containing the actual function call (e.g. mkdir). */ 
    char **args;

    for (int i=0; i<arrSize; i++) {

        if (arr[i] == NULL) {
            continue;
        }

        else {
            /* Process Control Block made for each command */
            struct ProcessControlBlock *pcb;

            char *line = arr[i];
            char *token = strtok(line, s);

            /* Save the first token in the variable command, because it will be
               used to indicate which UNIX system call is being referenced. */
            char *command = token;

            /* Allocate memory for the arguments. */
            size_t argSize = 5;
            args = (char**)malloc(argSize * sizeof(char*));

            /* Initialize values of args to NULL in case there are not enough 
               arguments given to occupy all of the allocated memory. */
            for (int i=0; i<argSize; i++) {
                args[i] = NULL;
            }

            /* Remove white space from the arguments of the call and then assign 
            the pointer 'args' */
            int idx = 0;
            while (token != NULL) {
                token = strtok(NULL, s);
                args[idx++] = token;
            }

            /* Get rid of './' characters from command string */
            strtok(command, "/");
            command = strtok(NULL, "");
        
            int pcbSize = idx + 1;
            pcb = malloc(pcbSize * sizeof(struct ProcessControlBlock));
            pcb->cmd = command;
            pcb->args = args;
            pcb->count = idx;
            pcb->state = READY; 

            /* Assign new PCB to PCBS & increment PCBS position */
            PCBS[PCBS_len] = pcb;
            PCBS_len++;
        }
    }
    return 1;
}

/* Free struct ProcessControlBlock */
int FreePCB(struct ProcessControlBlock **PCBS) {
    for (int i=0; i<PCBS_len; i++) {
        free(PCBS[i]->args);
        free(PCBS[i]);
    }
    free(PCBS);
    return 1;
}