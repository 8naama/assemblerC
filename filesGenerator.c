#include "dataStructures.h"
#include "filesGenerator.h"
#include "global.h"


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

    /* if failed to open the file, throws error */
    if (!currFile) {
        printf("Error: failed to open file %s\n" , filename);
        return;
    }
    
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
    char *objectFileName;
    objectFileName = (char *) malloc(strlen(filename)+strlen(OBJECT_FILE_TYPE)+1);
    sprintf(objectFileName, "%s%s", filename, OBJECT_FILE_TYPE);
    _writeToFile(objectFileName, newline);
    free(objectFileName);
}


/*
Writes the given line to the given filename as .EXTERNAL_FILE_TYPE file.

Input: filename of the file, key of the external var and the line it was called at
Output: None
*/
void writeToExternalFile(char filename[], char key[], int lineNumber)
{
    char *externalFileName, 
         newline[MAX_LINE_LEN];
    externalFileName = (char *) malloc(strlen(filename)+strlen(EXTERNAL_FILE_TYPE)+1);
    sprintf(externalFileName, "%s%s", filename, EXTERNAL_FILE_TYPE);
    sprintf(newline, "%s\t%d", key, lineNumber);
    _writeToFile(externalFileName, newline);
    free(externalFileName);
}

/*
Generates the .ENTRY_FILE_TYPE file based on the Symbols table.

Input: filename of the file
Output: None
*/
void generateEntryFile(char filename[])
{
    /* Initializing file name and the current Symbol */
    Symbol *currSymbol = symbolTableHead, 
           *tmp;
    char *entryFileName,
         newline[MAX_LABEL_NAME_LEN+6];

    entryFileName = (char *) malloc(strlen(filename)+strlen(ENTRY_FILE_TYPE)+1);
    sprintf(entryFileName, "%s%s", filename, ENTRY_FILE_TYPE);

    /* Going over all the Symbols */
    while (currSymbol) {


        /* If entry, then save to the entry file */
        if (currSymbol->method == entry) {
            sprintf(newline, "%s\t%d", currSymbol->name, currSymbol->value);
            _writeToFile(entryFileName, newline);
        }
        tmp = currSymbol->next;
        currSymbol = tmp;
    }
    free(entryFileName);
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
    wordEncoded = (char *) malloc(sizeof(char)*9);

    if (!wordEncoded) {
        printf("Error: Failed to allocate memory during encoding the words.\n");
        exit(1);
    }

    wordEncoded[0] = '\0';

    /* Translate base2 >> base4 >> special encoding */
    for (i=0; i < BINARY_WORD_SIZE; i+=2) {
        twoBinaryDigits[2] = '\0';
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
void addBinaryWordInObjectFile(char filename[], int decimalAddress, char binaryWord[]) 
{
    char newline[MAX_LINE_LEN];
    sprintf(newline, "%d\t%s", decimalAddress, _encodeWord(binaryWord));
    writeToObjectFile(filename, newline);
}

