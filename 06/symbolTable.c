#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbolTable.h"

#define STARTING_NEW_MEM_ADDR 16
#define SYMBOL_LENGTH 60
#define BITS 16


struct symbolTable staticMembers[] = 
    {
        {.symbol_name = "R0", .label = false, .memory_addr = 0, .next = &staticMembers[1]},
        {.symbol_name = "R1", .label = false, .memory_addr = 1, .next = &staticMembers[2]},
        {.symbol_name = "R2", .label = false, .memory_addr = 2, .next = &staticMembers[3]},
        {.symbol_name = "R3", .label = false, .memory_addr = 3, .next = &staticMembers[4]},
        {.symbol_name = "R4", .label = false, .memory_addr = 4, .next = &staticMembers[5]},
        {.symbol_name = "R5", .label = false, .memory_addr = 5, .next = &staticMembers[6]},
        {.symbol_name = "R6", .label = false, .memory_addr = 6, .next = &staticMembers[7]},
        {.symbol_name = "R7", .label = false, .memory_addr = 7, .next = &staticMembers[8]},
        {.symbol_name = "R8", .label = false, .memory_addr = 8, .next = &staticMembers[9]},
        {.symbol_name = "R9", .label = false, .memory_addr = 9, .next = &staticMembers[10]},
        {.symbol_name = "R10", .label = false, .memory_addr = 10, .next = &staticMembers[11]},
        {.symbol_name = "R11", .label = false, .memory_addr = 11, .next = &staticMembers[12]},
        {.symbol_name = "R12", .label = false, .memory_addr = 12, .next = &staticMembers[13]},
        {.symbol_name = "R13", .label = false, .memory_addr = 13, .next = &staticMembers[14]},
        {.symbol_name = "R14", .label = false, .memory_addr = 14, .next = &staticMembers[15]},
        {.symbol_name = "R15", .label = false, .memory_addr = 15, .next = &staticMembers[16]},
        {.symbol_name = "SCREEN", .label = false, .memory_addr = 16384, .next = &staticMembers[17]},
        {.symbol_name = "KBD", .label = false, .memory_addr = 24576, .next = &staticMembers[18]},
        {.symbol_name = "SP", .label = false, .memory_addr = 0, .next = &staticMembers[19]},
        {.symbol_name = "LCL", .label = false, .memory_addr = 1, .next = &staticMembers[20]},
        {.symbol_name = "ARG", .label = false, .memory_addr = 2, .next = &staticMembers[21]},
        {.symbol_name = "THIS", .label = false, .memory_addr = 3, .next = &staticMembers[22]},
        {.symbol_name = "THAT", .label = false, .memory_addr = 4, .next = NULL},
    };

struct symbolTable *head = &staticMembers[0];
struct symbolTable *final = &staticMembers[22];

void createSymbolEntryLabel(int nextLine, const char *symbolName){
    struct symbolTable *newNode; 
    if ((newNode = (struct symbolTable *) malloc(sizeof(struct symbolTable))) == NULL){
        fprintf(stderr, "Error: Malloc failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->symbol_name, symbolName);
    newNode->memory_addr = nextLine;
    newNode->label = true;
    newNode->next = NULL;

    final->next = newNode;
    final = final->next;
}

void createSymbolEntry(const char *symbolName){
    int freeMemoryAddress = STARTING_NEW_MEM_ADDR;
    struct symbolTable *p = staticMembers[22].next;
    while (p != NULL){
        // Symbol node can take the same memory address as a label node
        if (p->label == false){
            if (p->memory_addr == freeMemoryAddress || freeMemoryAddress == staticMembers[16].memory_addr || 
                freeMemoryAddress == staticMembers[17].memory_addr){
                freeMemoryAddress++;
                p = staticMembers[22].next;
            }
            else{
                p = p->next;
            }
        }
        else{
            p = p->next;
        }     
    }

    struct symbolTable *newNode; 
    if ((newNode = (struct symbolTable *) malloc(sizeof(struct symbolTable))) == NULL){
        fprintf(stderr, "Error: Malloc failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->symbol_name, symbolName);
    newNode->memory_addr = freeMemoryAddress;
    newNode->next = NULL;

    final->next = newNode;
    final = final->next;
}

void printSymbolTable(void){
    struct symbolTable *p = &staticMembers[0];
    while (p != NULL){
        printf("Name: %-35s Memory Addr: %-8d Label: %-1d\n", p->symbol_name, p->memory_addr, p->label);
        p = p->next;
    }
}

struct symbolTable *searchSymbol(const char *name){
    struct symbolTable *p = head;
    while (p != NULL){
        if (strcmp(p->symbol_name, name) == 0){
            return p;
        }
        p = p->next;
    }
    return NULL;
}