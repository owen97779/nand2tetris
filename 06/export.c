#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "export.h"

#define BIN_FILE "output.hack"

struct machineCode *final_machine, *head_machine = NULL;

void createMachineNode(char *binaryString){
    struct machineCode *node = (struct machineCode *) malloc(sizeof(struct machineCode));
    if(strncpy(node->binary, binaryString, BITS) == NULL){
        fprintf(stderr, "Error: Cannot copy machine code binary string to node\n");
        exit(EXIT_FAILURE);
    }
    node->next = NULL;
    if (head_machine == NULL){
        head_machine = node;
        final_machine = node;
    }
    else {
        final_machine->next = node;
        final_machine = final_machine->next;
    }
}

void createBin(void){
    FILE *fp;

    fp = fopen(BIN_FILE, "w");
    struct machineCode *p = head_machine;
    while (p != NULL){
        fwrite(p->binary, sizeof(char), BITS, fp);
        fputc('\n', fp);
        p = p->next;
    }
    fclose(fp);
}
