#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "limitations.h"

#define BINARY_WORD_SIZE 14
#define MACRO_OUTPUT_FILE_TYPE ".am"
#define ASSEMBLY_FILE_TYPE ".as"
#define COMMENT_SIGN ";"
#define LABEL_SIGN ":"


enum lineType {
    code0,
    code1,
    code2,
    dataLine,
    none
};


int startsWith(char *word, char *substring);
enum lineType findInstructionType(char line[]);
int isRegistry(char arg[]);
void verifyInput(int argc, char *argv[]);
char *removeExtentionFromName(char filename[]);


