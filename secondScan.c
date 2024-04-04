#include "secondScan.h"
#include "dataStructures.h"


void myfunc(char name[]) {
    /* Example 1: Use findInSymbolsTable() with var defined inside the function */
    char n[] = "LOOP";
    Symbol *alreadyExists;

    alreadyExists = findInSymbolsTable(n);

    if (alreadyExists)
        printf("Example 1: Symbol %s with value %d\n", alreadyExists->name, alreadyExists->value);
    else printf("Example 1: did not find %s\n", n);

    /* Example 2: Use findInSymbolsTable() with var passed to the function */
    alreadyExists = findInSymbolsTable(name);

    if (alreadyExists)
        printf("Example 2: found %s with value %d\n", alreadyExists->name, alreadyExists->value);
    else printf("Example 2: did not find %s\n", name);

    /* Example 3: Use symbolTableHead directly */
    alreadyExists = symbolTableHead;
    printf("Example 3: found %s with value %d\n", alreadyExists->name, alreadyExists->value);
}
