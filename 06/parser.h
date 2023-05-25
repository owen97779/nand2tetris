#ifndef PARSER_H
#define PARSER_H

#define BITS 16

// Determines the type of instruction on input string
char instructionType(char *);
// First pass, counts lines and adds labels to symbolTable
void firstPass(const char *);
// Second pass, calls functions
void secondPass(const char *);
// Creates machine code and adds it to the machine code struct
void symbol(char, char *);
// Function for A instruction
char *aInstruction(char *);
// Functions for C instructions
const char *dest(char *);
const char *comp(char *);
const char *jump(char *);
// Converts int (hopefully in binary form) to string
void intToBinaryString(int, char*, int);

#endif