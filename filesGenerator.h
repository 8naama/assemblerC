#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENTRY_FILE_TYPE ".en"
#define EXTERNAL_FILE_TYPE ".ex"
#define OBJECT_FILE_TYPE ".ob"


void writeToObjectFile(char filename[], char newline[]);
void writeToExternalFile(char filename[], char key[], int lineNumber);
void generateEntryFile(char filename[]);

void generateFiles(char filename[], struct Symbol *symbolsHead, int instCount, int dataCount, struct MemoryData *binaryWordHead);
int main();
