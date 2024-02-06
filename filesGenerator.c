#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructures.h"

#define ENTRY_FILE_TYPE ".en"
#define EXTERNAL_FILE_TYPE ".ex"
#define OBJECT_FILE_TYPE ".ob"

FILE *currFile;


void _writeToFile(char filename[], char newline[]) 
{
    currFile = fopen(filename, "a");
    fprintf(currFile, "%s\n", newline);
    fclose(currFile);
}


void _generateEntryAndExternalFiles(char filename[], struct Symbol *symbolsHead)
{
    /* Initializing file names and the current Symbol */
    char entryFileName[100];
    char externalFileName[100];
    Symbol *currSymbol = symbolsHead;
    Symbol *tmp;
    sprintf(entryFileName, "%s%s", filename, ENTRY_FILE_TYPE);
    sprintf(externalFileName, "%s%s", filename, EXTERNAL_FILE_TYPE);

    /* Going over all the Symbols */
    while (currSymbol) {
        enum SymbolUpdateMethod currMethod = currSymbol->method;
        
        char newline[MAX_LABEL_NAME_LEN+6];
        sprintf(newline, "%s\t%d", currSymbol->name, currSymbol->value);
        
        /* If external or entry, then save to the relevant file */
        if (currMethod == entry)
            _writeToFile(entryFileName, newline);
        else if (currMethod == external)
            _writeToFile(externalFileName, newline);

        tmp = currSymbol->next;
        /*free(currSymbol);*/ /* TODO: check how memory cleaning works*/
        currSymbol = tmp;
    }
}


char *_encodeWord(char word)
{
    printf("skip encode word\n");
    return "*&!##!*";
}


void _generateObjectFile(char filename[], int instCount, int dataCount, struct MemoryData *binaryWordHead) 
{
    /* Initializing file names and the current Symbol */
    char objectFileName[100];
    char header[5];
    MemoryData *currLine = binaryWordHead;
    MemoryData *tmp;
    sprintf(objectFileName, "%s%s", filename, OBJECT_FILE_TYPE);
    sprintf(header, "\t%d %d", instCount, dataCount);
    
    /* Writing Header to the Object file */
    _writeToFile(objectFileName, header);

    /* Going over all the memory lines */    
    while (currLine) {
        char newline[50];

        sprintf(newline, "%d\t%s", currLine->decimalAddress, _encodeWord(currLine->binary));
        _writeToFile(objectFileName, newline);

        tmp = currLine->next;
        /*free(currLine);*/ /* TODO: check how memory cleaning works*/
        currLine = tmp;
    }
}


/*
Generates files named after the given 'filename', with file types matching ENTRY_FILE_TYPE, EXTERNAL_FILE_TYPE and OBJECT_FILE_TYPE.
The files content depends on the given 'symbols_head' and 'binary_word_head' structures content.

Input: string filename, Symbol structure, instructions lines count, data lines count and MemoryData structure.
Output: None, but generates between 1 to 3 files, depending on the given structures content.
*/
void generateFiles(char filename[], struct Symbol *symbolsHead, int instCount, int dataCount, struct MemoryData *binaryWordHead)
{
    _generateEntryAndExternalFiles(filename, symbolsHead);
    _generateObjectFile(filename, instCount, dataCount, binaryWordHead);
}


/*
This main function is temporary for test purpose.
*/
int main()
{
    enum SymbolUpdateMethod ex = external;
    enum SymbolUpdateMethod en = entry;
    enum SymbolType el = code;

    /*Symbol first, second, third;*/
    Symbol first = {"sz", el, en, 23};
    Symbol second = {"X", el, en, 200, &first};
    Symbol third = {"LINE", el, ex, 10, &second};

    MemoryData test = {101, "101011001110"};
    MemoryData test2 = {100, "010110011101", &test};

    printf("main test run\n");
    generateFiles("justatest", &third, 10, 11, &test2);
    printf("did it work?\n");
    return 0;
}

