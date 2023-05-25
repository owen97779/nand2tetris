#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "symbolTable.h"
#include "export.h"

#define BITS 16
#define BUFFER_SIZE 64

/* Tables may contain duplicate values to account for combinations */

const char *destTable[15][2] =    {{"M", "001"},
                            {"D", "010"},
                            {"DM", "011"},
                            {"MD", "011"},
                            {"A", "100"},
                            {"AM", "101"},
                            {"MA", "101"},
                            {"AD", "110"},
                            {"DA", "110"},
                            {"ADM", "111"},
                            {"AMD", "111"},
                            {"MAD", "111"},
                            {"MDA", "111"},
                            {"DMA", "111"},
                            {"DAM", "111"}};

const char *jumpTable[7][2] =     {{"JGT", "001"},   
                            {"JEQ", "010"},
                            {"JGE", "011"},
                            {"JLT", "100"},
                            {"JNE", "101"},
                            {"JLE", "110"},
                            {"JMP", "111"}};

const char *compTable[28][2] =    {{"0", "1110101010"},
                            {"1", "1110111111"},
                            {"-1", "1110111010"},
                            {"D", "1110001100"},
                            {"A", "1110110000"},
                            {"M", "1111110000"},
                            {"!D", "1110001101"},
                            {"!A", "1110110001"},
                            {"!M", "1111110001"},
                            {"-D", "1110001111"},
                            {"-A", "1110110011"},
                            {"-M", "1111110011"},
                            {"D+1", "1110011111"},
                            {"A+1", "1110110111"},
                            {"M+1", "1111110111"},
                            {"D-1", "1110001110"},
                            {"A-1", "1110110010"},
                            {"M-1", "1111110010"},
                            {"D+A", "1110000010"},
                            {"D+M", "1111000010"},
                            {"D-A", "1110010011"},
                            {"D-M", "1111010011"},
                            {"A-D", "1110000111"},
                            {"M-D", "1111000111"},
                            {"D&A", "1110000000"},
                            {"D&M", "1111000000"},
                            {"D|A", "1110010101"},
                            {"D|M", "1111010101"}};

char instructionType(char *line){
    char *p = line;
    // Skips whitespace
    while (isspace(*p)){
        p++;
    }
    // Instruction type A
    if (*p == '@' && isalnum(*(p + 1))){
        return 'A';
    }
    // Instruction type C
    else if (*p == '0' || *p == '1' || *p == '-' || 
            *p == '!' || *p == '-' || *p == '!' ||
            *p == 'A' || *p == 'D' || *p == 'M'){
        
        return 'C';
    }
    // Label, assuming the closed bracket is formatted correctly
    else if (*p == '('){
        return 'L';
    }
    // Invalid character or comment
    else {
        return '\0';
    }
}

void symbol(char type, char *line){
    char machineCode[BITS + 1];
    memset(machineCode, 0, sizeof(machineCode));

    if (type == 'A'){
        char *aMalloc = aInstruction(line);
        strcat(machineCode, aMalloc);
        createMachineNode(machineCode);
        // A instruction function returns a dynamic string
        free(aMalloc);
    }
    else if (type == 'C'){
        // C instructions point to values in tables
        strcat(machineCode, comp(line));
        strcat(machineCode, dest(line));
        strcat(machineCode, jump(line));
        createMachineNode(machineCode);
    }
}

char *aInstruction(char *line){
    // 16 bit A instruction 
    char *aMachineCode;
    if ((aMachineCode = (char *) malloc((BITS + 1) * sizeof(char))) == NULL){
        fprintf(stderr, "Error: Could not malloc A instruction\n");
        exit(EXIT_FAILURE);
    }
    memset(aMachineCode, 0, BITS + 1);
    // Parse the symbol variable name 
    char *token = strstr(line, "@");
    char *end = token;
    while (!isspace(*end)){
        end++;
    }
    if (token == NULL || end == NULL){
        fprintf(stderr, "Error: Cannot parse @ symbol and end of variable name\n");
        exit(EXIT_FAILURE);
    }
    int nameLength = end - token;
    char *variableSymbolName;
    if ((variableSymbolName = (char *) malloc((nameLength + 1) * sizeof(char))) == NULL){
        fprintf(stderr, "Error: Could not malloc for variable symbol name\n");
        exit(EXIT_FAILURE);
    }
    strncpy(variableSymbolName, token + 1, nameLength - 1);
    variableSymbolName[nameLength] = '\0';
    // Does the symbol name exist
    struct symbolTable *p = searchSymbol(variableSymbolName);
    // If symbol name exists
    if (p != NULL){
        intToBinaryString(p->memory_addr, aMachineCode, BITS);
    }
    else{
        // If symbol name is an integer
        char *ptr;
        long int memoryAddress = strtol(variableSymbolName, &ptr, 10);
        if (*ptr == '\0' && ptr != variableSymbolName){
            intToBinaryString(memoryAddress, aMachineCode, BITS);
        }
        // Not an integer, add symbol to table, it assigns next available memory address
        else{
            createSymbolEntry(variableSymbolName);
            p = searchSymbol(variableSymbolName);
            intToBinaryString(p->memory_addr, aMachineCode, BITS);    
        }
    }
    free(variableSymbolName);
    return aMachineCode;
}

const char *dest(char *line){
    // 111accccccdddjjj
    const char *destNull = "000";
    char *equals = strstr(line, "=");
    if (equals == NULL){
        return destNull;
    }
    char *start = line;
    // skips to first character ignoring whitespace
    while (isspace(*start)){
        start++;
    }
    int instLength = equals - start;
    if (instLength < 1){
        fprintf(stderr, "Error: Instruction not valid\n");
        exit(EXIT_FAILURE);
    }
    char *instruction = (char *) malloc((instLength + 1) * sizeof(char));
    strncpy(instruction, start, instLength);
    instruction[instLength] = '\0';
    for (int i = 0; i < 15; i++){
        if (strncmp(instruction, destTable[i][0], instLength) == 0){
            free(instruction);
            return destTable[i][1];
        }
    }
    free(instruction);
    return destNull;
}

const char *jump(char *line){
    // 111accccccdddjjj
    const char *jumpNull = "000";
    char *semiColon = strstr(line, ";");
    if (semiColon == NULL){
        return jumpNull;
    }
    char *p = semiColon;
    // skips to whitespace
    while (!isspace(*p)){
        p++;
    }
    int instLength = p - semiColon;
    if (instLength < 1){
        fprintf(stderr, "Error: Instruction not valid\n");
        exit(EXIT_FAILURE);
    }
    char *instruction = (char *) malloc((instLength + 1) * sizeof(char));
    strncpy(instruction, ++semiColon, instLength);
    instruction[instLength] = '\0';
    for (int i = 0; i < 7; i++){
        if (strncmp(instruction, jumpTable[i][0], instLength - 1) == 0){
            free(instruction);
            return jumpTable[i][1];
        }
    }
    free(instruction);
    return jumpNull;
}

const char *comp(char *line){
    // 111accccccdddjjj
    const char *p = line;
    char *equals = strstr(line, "=");
    char *semicolon = strstr(line, ";");
    int instLength = 0;
    if (equals != NULL && semicolon != NULL){
        //Assume no whitespace between equals and semicolon
        p = equals + 1;
        instLength = semicolon - ++equals; 
    }
    else if (equals == NULL){
        while (isspace(*p)){
            p++;
        }
        instLength = semicolon - p;
    }
    else if (semicolon == NULL){
        p = equals + 1;
        while (!isspace(*p)){
            p++;
        }
        instLength = p - equals - 1;
        p = equals + 1;
        
    }
    char *instruction = (char *) malloc((instLength + 1) * sizeof(char));
    strncpy(instruction, p, instLength);
    instruction[instLength] = '\0';
    for (int i = 0; i < 28; i++){
        if (strncmp(instruction, compTable[i][0], instLength) == 0){
            free(instruction);
            return compTable[i][1];
        }
    }
    free(instruction);
    fprintf(stderr, "Could not find valid C comp instruction\n");
    exit(EXIT_FAILURE);
    return NULL; 
}

void firstPass(const char *filename){
    FILE *fp;
    char symbolName[SYMBOL_LENGTH];
    char stringLineBuffer[BUFFER_SIZE];
    int lines = 0;

    // Could not open the asm file
    if ((fp = fopen(filename, "r")) == NULL){
        fprintf(stderr, "Error: Could not open assemnly file\n");
        exit(EXIT_FAILURE);
    }
    
    // Read asm file line by line
    while (fgets(stringLineBuffer, BUFFER_SIZE, fp) != NULL){
        char type = instructionType(stringLineBuffer);
        // If valid instruction or label
        if (type != '\0'){
            // If label, add label name to symbolTable
            if (type == 'L'){
                char *open_bracket = strstr(stringLineBuffer, "(");
                char *closed_bracket = strstr(stringLineBuffer, ")");
                if (closed_bracket == NULL){
                    fprintf(stderr, "Error: Could not find closing bracket for label\n");
                    exit(EXIT_FAILURE);
                }
                int i = 0;
                for (char *p = open_bracket + 1; p < closed_bracket && i < SYMBOL_LENGTH - 1;p++){
                    symbolName[i++] = *p;
                }
                symbolName[i] = '\0';
                createSymbolEntryLabel(lines, symbolName);
            }
            else {
                // Count line as it is an A or C instruction
                lines++;
            }
        }
    }
    fclose(fp);
}


void secondPass(const char *filename){
    FILE *fp;
    char stringLineBuffer[BUFFER_SIZE];

    if ((fp = fopen(filename, "r")) == NULL){
        fprintf(stderr, "Error: Could not open assemnly file\n");
        exit(EXIT_FAILURE);
    }
    
    while (fgets(stringLineBuffer, BUFFER_SIZE, fp) != NULL){
        char type = instructionType(stringLineBuffer);
        symbol(type, stringLineBuffer);
    }
    fclose(fp);
}

void intToBinaryString(int num, char* buffer, int bufferSize) {
    // Make sure bufferSize is sufficient for the binary representation of num
    if (bufferSize <= 0) {
        fprintf(stderr, "Error: Invalid buffer size\n");
        exit(EXIT_FAILURE);
    }
    // Set all buffer elements to '0' initially
    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = '0';
    }
    // Convert num to binary representation
    int index = bufferSize - 1;
    while (num > 0) {
        buffer[index] = (num % 2) + '0'; // Convert remainder to character '0' or '1'
        num /= 2;
        index--;
    }
    buffer[bufferSize] = '\0';
}