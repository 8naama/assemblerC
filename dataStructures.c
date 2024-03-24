#include "dataStructures.h"


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
