#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENTRY_FILE_TYPE ".en"
#define EXTERNAL_FILE_TYPE ".ex"
#define OBJECT_FILE_TYPE ".ob"

char *SPECIAL_ENCODING[4] = {"*", "#", "%", "!"};

void generateFiles(char filename[], struct Symbol *symbolsHead, int instCount, int dataCount, struct MemoryData *binaryWordHead);
int main();
