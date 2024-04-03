#include "dataStructures.h"


/*
Searches for a symbol with the given name in the global Table, starting from symbolTableHead.

Input: String name of ymbol to search for.
Output: Pointer to the Symbol with the given name, or Null if it doesn't exist in the table.
*/
Symbol *findInSymbolsTable(char name[])
{
    Symbol *currSymbol = symbolTableHead;

    while (currSymbol) {
        if (strcmp(currSymbol->name, name) == 0)  /* currSymbol.name = given name */
            return currSymbol;
        currSymbol = currSymbol->next;
    }
    return NULL;
}


/* test function: to be removed in the final version */
void printSymbols() {
    Symbol *curr = symbolTableHead;
        char method[20] = "", type[10] = "";

    while (curr) {
        if (curr->method == external)
            strcpy(method, "external");
        else if (curr->method == entry)
            strcpy(method, "entry");
        else
            strcpy(method, "relocatable");

        if (curr->type == code)
            strcpy(type, "code");
        else if (curr->type == data)
            strcpy(type, "data");
        else if (curr->type == mdefine)
            strcpy(type, "mdefine");

        printf("row; %s | %d | %s | %s\n", curr->name, curr->value, method, type);
        curr = curr->next;
    }
}
