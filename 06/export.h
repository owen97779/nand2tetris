#ifndef EXPORT_H
#define EXPORT_H

#define BITS 16

// Linked list for machine code
struct machineCode{
    // Can only be an A or C instruction
    char binary[BITS];
    struct machineCode *next;
};

// Create a linked list node 
void createMachineNode(char *);
// Create the machine code file
void createBin(void);
#endif