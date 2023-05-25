#include <stdbool.h>
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#define SYMBOL_LENGTH 60
#define BITS 16

// Linked list symbol table
struct symbolTable {
    char symbol_name[SYMBOL_LENGTH];
    bool label;
    short unsigned int memory_addr;
    struct symbolTable *next;
};

// Creating a label entry
void createSymbolEntryLabel(int, const char *);
// Creating a symbol entry
void createSymbolEntry(const char *);
// Search for symbol
struct symbolTable *searchSymbol(const char *);
// Print all values in linked list symbol table
void printSymbolTable(void);

#endif