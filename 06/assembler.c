#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "parser.h"
#include "export.h"

int main(int argc, char **argv){
    /* Check if asm file is provided */
    if (argc < 2){
        fprintf(stderr, "Error: Did not provide assembly file\n");
        exit(EXIT_FAILURE);
    }

    firstPass(argv[1]);
    secondPass(argv[1]);
    createBin();

    /* Logging functions */
    printSymbolTable();

    return 0;
}

