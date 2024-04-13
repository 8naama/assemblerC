#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LABEL_NAME_LEN 31

enum SymbolType {
    mdefine,
    code,
    data
};

enum SymbolUpdateMethod {
    external,
    entry,
    relocatable
};

typedef struct Symbol {
    char name[MAX_LABEL_NAME_LEN];
    enum SymbolType type;
    enum SymbolUpdateMethod method;
    int value;
    struct Symbol *next;
} Symbol;

struct Symbol *symbolTableHead;

Symbol *findInSymbolsTable(char name[]);
void printSymbols(); /* test func - to remove in final version */

