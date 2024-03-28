#include "dataStructures.h"


/*
Searches for a symbol with the given name in the global Table, starting from symbolTableHead.

Input: String name of ymbol to search for.
Output: Pointer to the Symbol with the given name, or Null if it doesn't exist in the table.
*/
Symbol *findInSymbolsTable(char name[])
{
    Symbol *currSymbol = symbolTableHead;

    while (currSymbol != NULL) {
        if (strcmp(currSymbol->name, name) == 0)  /* currSymbol.name = given name */
            return currSymbol;
            currSymbol = currSymbol->next;
    }
    return NULL;
}
