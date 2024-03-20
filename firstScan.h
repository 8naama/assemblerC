#include <string.h>

#define READ_FILE_TYPE ".am"
#define MAX_LINE_LEN 100  // TODO: MAKE SINGLE FILE FOR GENERAL VARS USED ACROSS THE PROGRAM
#define COMMENT_SIGN ';'
#define LABEL_SIGN ':'


enum lineType {
    code0,
    code1,
    code2,
    data,
    none
};

struct Symbol symbolTable;

char *commandTwoArgs[5] = {"mov", "cmp", "add", "sub", "lea"};
char *commandOneArgs[9] = {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
char *commandNoArgs[2] = {"rts", "hlt"};
char *definitionAndDirective[5] = {".data", ".string", ".entry", ".extern", ".define"};
char *registriesNames[8] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
