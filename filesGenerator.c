#include "dataStructures.h"
#include "filesGenerator.h"


/* Initialize variables */
FILE *currFile;
char *SPECIAL_ENCODING[4] = {"*", "#", "%", "!"};


/*
Writes the given newline to the given filename.

Input: filename of the file, newline to add to the file
Output: None
*/
void _writeToFile(char filename[], char newline[]) 
{
    currFile = fopen(filename, "a");
    fprintf(currFile, "%s\n", newline);
    fclose(currFile);
}


/*
Writes the given newline to a given filename as .OBJECT_FILE_TYPE file.

Input: filename of the file, newline to add to the file
Output: None
*/
void writeToObjectFile(char filename[], char newline[])
{
    char objectFileName[strlen(filename)+strlen(OBJECT_FILE_TYPE)+1];
    sprintf(objectFileName, "%s%s", filename, OBJECT_FILE_TYPE);
    _writeToFile(objectFileName, newline);
}


void writeToExternalFile(char filename[], char key[], int lineNumber)
{
    char externalFileName[strlen(filename)+strlen(EXTERNAL_FILE_TYPE)+1], 
         newline[strlen(key)+5];
    sprintf(externalFileName, "%s%s", filename, EXTERNAL_FILE_TYPE);
    sprintf(newline, "%s\t%d", key, lineNumber);

    _writeToFile(externalFileName, newline);
}


void generateEntryFile(char filename[])
{
    /* Initializing file name and the current Symbol */
    enum SymbolUpdateMethod currMethod;
    Symbol *currSymbol = symbolTableHead, 
           *tmp;
    char entryFileName[strlen(filename)+strlen(ENTRY_FILE_TYPE)+1],
         newline[MAX_LABEL_NAME_LEN+5];
    sprintf(entryFileName, "%s%s", filename, ENTRY_FILE_TYPE);
    _writeToFile(entryFileName, newline);

    /* Going over all the Symbols */
    while (currSymbol) {
        currMethod = currSymbol->method;
    
        char newline[MAX_LABEL_NAME_LEN+6];
        /* If entry, then save to the entry file */
        if (currMethod == entry) {
            sprintf(newline, "%s\t%d", currSymbol->name, currSymbol->value);
            _writeToFile(entryFileName, newline);
        }

        tmp = currSymbol->next;
        currSymbol = tmp;
    }
}


/*
Genrates EXTERNAL_FILE_TYPE and ENTRY_FILE_TYPE based on the given Symbol structure.
If there are no symbols with entry as method, does not generate the relevant file.
Same behaviour happens for external method.

Input: filename of the new files, Symbol structure
Output: None
*/
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
        /*free(currSymbol);*/ /* Uncomment this in final run, when the data structures are taking up memory */
        currSymbol = tmp;
    }
}


/*
Encodes the given word based on base2 >> base4 >> SPECIAL_ENCODING.

Input: word in base 2 to encode
Output: word encoded
*/
char *_encodeWord(char word[])
{
    /* Initializing variables */
    int i;
    char twoBinaryDigits[3];
    char *wordEncoded;
    wordEncoded = (char *) malloc(8);

    /* Translate base2 >> base4 >> special encoding */
    for (i=0; i < 14; i+=2) {
        twoBinaryDigits[0] = '\0';
        strncpy(twoBinaryDigits, word, 2);
        word += 2;

        if (strcmp(twoBinaryDigits, "00") == 0)
            strcat(wordEncoded, SPECIAL_ENCODING[0]);  /* 00 = 0 */
        else if (strcmp(twoBinaryDigits, "01") == 0)
            strcat(wordEncoded, SPECIAL_ENCODING[1]);  /* 01 = 1 */
        else if (strcmp(twoBinaryDigits, "10") == 0)
            strcat(wordEncoded, SPECIAL_ENCODING[2]);  /* 10 = 2 */
        else if (strcmp(twoBinaryDigits, "11") == 0)
            strcat(wordEncoded, SPECIAL_ENCODING[3]);  /* 11 = 3 */
    }
    return wordEncoded;
}


/*
Add to the OBJECT_FILE_TYPE file the encoded words from binaryWordHead lines encoded.

Input: filename of the new file, instCount (number of instructions), dataCount (number of data lines) and MemoryData structure.
Output: None
*/
void _generateObjectFile(char filename[], int instCount, int dataCount, struct MemoryData *binaryWordHead) 
{
    /* Initializing file names and the current Symbol */
    char objectFileName[100];
    MemoryData *currLine = binaryWordHead;
    MemoryData *tmp;
    sprintf(objectFileName, "%s%s", filename, OBJECT_FILE_TYPE);

    /* Going over all the memory lines */    
    while (currLine) {
        char newline[50];

        snprintf(newline, 50, "%d\t%s", currLine->decimalAddress, _encodeWord(currLine->binary));
        printf("%s\n", newline);
        _writeToFile(objectFileName, newline);

        tmp = currLine->next;
        /*free(currLine);*/ /* Uncomment this in final run, when the data structures are taking up memory */
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
