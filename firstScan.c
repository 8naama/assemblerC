#include "dataStructures.h"
#include "filesGenerator.h"
#include "firstScan.h"


/* Initialize variables relevant to the first scan */
char *commandTwoArgs[5] = {"mov", "cmp", "add", "sub", "lea"};
char *commandOneArgs[9] = {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
char *commandNoArgs[2] = {"rts", "hlt"};
char *definitionAndDirective[5] = {".data", ".string", ".entry", ".extern", ".define"};
char *registriesNames[8] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

int currDecimalAddr = 100;  /* the decimal addresses counter */
struct Symbol *symbolTableNext;  /* pointer to mark place at the table */

/* variables for the Object file's header  */
int instructionsSectionLen = 0;
int dataSectionLen = 0;


/*
If given word starts with given substring, return 1 otherwise return 0.

Input: word to check if starts with given substring
Output: 1 if word starts with substring, 0 otherwise
*/
int _startsWith(char *word, char *substring)
{
    if(strncmp(word, substring, strlen(substring)) == 0)
        return 1;
    return 0;
}


/*
Returns if the given Strings array contains the given word.

Input: Array of strings and a word string.
Output: 1 if array contains the word, 0 otherwise
*/
int _stringArrayContains(char *arr[], int arrLen, char word[])
{
    int i;
    for (i=0; i < arrLen; i++) {
        if (strcmp(arr[i], word) == 0)
            return 1;
    }
    return 0;
}


/*
Returns if the given line contains a word from the given array.

Input: Array of strings, it's length and a string line.
Output: 1 if the line contains a word from the array, 0 otherwise
*/
int _lineContainsOneOf(char *arr[], int arrLen, char line[])
{
    int i;
    for (i=0; i <= arrLen; i++) {
        if (strstr(line, arr[i]))
            return 1;
    }
    return 0;
}

/*
Returns if the given word is in the definitionAndDirective array.

Input: string word.
Output: 1 if the word means the line is data line, 0 otherwise
*/
int _isData(char line[])
{
    int length = sizeof(definitionAndDirective) / sizeof(definitionAndDirective[0]) - 1;
    return _lineContainsOneOf(definitionAndDirective, length, line);
}


/*
Returns if the given word is a known code word and it's type:
- code0: no args
- code1: 1 arg
- code2: 2 args

Input: string word.
Output: code type if the word is code, NULL otherwise
*/
enum lineType _isCode(char line[])
{
    int length0 = sizeof(commandNoArgs) / sizeof(commandNoArgs[0]) - 1,
        length1 = sizeof(commandOneArgs) / sizeof(commandOneArgs[0]) - 1,
        length2 = sizeof(commandTwoArgs) / sizeof(commandTwoArgs[0]) - 1;

    if (_lineContainsOneOf(commandNoArgs, length0, line))
        return code0;
    else if (_lineContainsOneOf(commandOneArgs, length1, line))
        return code1;
    else if (_lineContainsOneOf(commandTwoArgs, length2, line))
        return code2;
    return none;
}


/*
Returns the given line instruction type.

Input: String line.
Output: code type, data type or NULL for comments and empty lines
*/
enum lineType _findInstructionType(char line[])
{
    enum lineType codeType;

    if (_startsWith(line, COMMENT_SIGN))
        return none;
    else if (_isData(line))
        return dataLine;
    codeType = _isCode(line);
        return codeType;
    return none;
}


/*
Recieves information for a new symbol to save, make sure it doesn't exist yet and saves it.
Since .entry and .extern are separate from a Symbol value and type definitions, handles updating the
Symbol's method or it's value and type accordingly.

Input: the new symbol info; name ,type (data or code), method (external, entry, relocatable) and it's value.
Output: 0 if saved successfuly, 1 otherwise.
*/
int _saveSymbolToTable(char name[], enum SymbolType type, enum SymbolUpdateMethod method, int value) {
    Symbol *alreadyExists, *newItem;

    alreadyExists = findInSymbolsTable(name);

    /* new Symbol */
    if (!alreadyExists) {

        if (!symbolTableHead) {  /* The first item in the table */
            symbolTableHead = (struct Symbol *)malloc(sizeof(enum SymbolUpdateMethod) * sizeof(enum SymbolType) * sizeof(struct Symbol));
            strcpy(symbolTableHead->name, name);
            symbolTableHead->type = type;
            symbolTableHead->method = method;
            symbolTableHead->value = value;
            symbolTableHead->next = NULL;
            
            symbolTableNext = symbolTableHead;
        }
        else {
        newItem = (Symbol*) malloc(sizeof(Symbol));
        strcpy(newItem->name, name);
        newItem->type = type;
        newItem->method = method;
        newItem->value = value;
        newItem->next = NULL;

        symbolTableNext->next = newItem;
        symbolTableNext = newItem;
}
    }
    else {
        /* already saved the symbol's value and type, now update that it's .entry or .extern */
        if ((method == entry || method == external) && alreadyExists->method == relocatable)
            alreadyExists->method = method;
        /* already saved that the symbol as .entry or .extern, now update the value and line type*/
        else if (alreadyExists->method == entry || alreadyExists->method == external) {
            alreadyExists->type = type;
            alreadyExists->value = value;
        }
        else {
            printf("Error: symbol %s already exists in symbol table and cannot be defined again.", name);
            return 1;
        }
    }
    return 0;
}


/*
Searchs for a label in the given line, if finds one, returns it, otherwise, return NULL.

Input: String line
Output: String Label value if found one, NULL otherwise.
*/
char *_findLabel(char line[])
{
    char *label, *tempLine;

    /* copy the line to a temp field to not edit org line and search for label */
    tempLine = (char *) malloc(strlen(line)+1);
    strcpy(tempLine, line);
    label = strtok(tempLine, LABEL_SIGN);

    /* label was found, verify length limitation */
    if (strcmp(label, line) != 0 && strlen(label) > MAX_LABEL_NAME_LEN) {
        printf("Error: label %s is passing the allowed %d characters limit.\n", label, MAX_LABEL_NAME_LEN);
        exit(1);
    }
    /* valid label was found, return it */
    if (strcmp(label, line) != 0)
        return label;
    return NULL;
}


/*
Recieves a line that contains a known action from definitionAndDirective:
1. collects the needed information on it
2. updates the amount of decimal adress lines the command will take
3. attempts to save or update a Symbol in the table according to [1]

Input: String line
Output: 0 if successful, 1 otherwise.
*/
int _handleDataLine(char line[])
{
    /* initializing variables */
    int value = -1,
        commaCount = 1,
        isSuccessful;
    char key[MAX_LABEL_NAME_LEN], currAction[7], *string, *ptr;
    enum SymbolType type = data;
    enum SymbolUpdateMethod method = relocatable;

    /* get needed variables from each action type */
    if (_startsWith(line, ".define")) {
        sscanf(line, ".define %s = %d", key, &value);
        type = mdefine;
    }
    else if (_startsWith(line, ".entry")) {
        sscanf(line, ".entry %s", key);
        method = entry;
    }
    else if (_startsWith(line, ".extern")) {
        sscanf(line, ".extern %s", key);
        method = external;
    }
    else {
        /* initialize the symbol value */  
        value  = currDecimalAddr;

        /* check if .data or .string */  
        sscanf(line, "%[^:]: .%s", key, currAction);
        string = (char *) malloc(MAX_LINE_LEN-strlen(key)+1);

        /* get needed variables from each action type */
        if (strcmp(currAction, "string") == 0) {
            sscanf(line, "%[^:]: .string %*[\"“]%[^\"“]%*[\"”]", key, string);
            currDecimalAddr += strlen(string) + 1;  /* +1 for '\0' */
            dataSectionLen += strlen(string) + 1;
        }
        else { /* currAction = data */
            sscanf(line, "%[^:]: .data %[^\n]", key, string);

            /* count the commas in the data params */
            ptr = string;
            while ((ptr = strchr(ptr, ','))) {
                commaCount++;
                ptr++;
            }
            currDecimalAddr += commaCount;
            dataSectionLen += commaCount;
        }
        free(string);
    }
    isSuccessful = _saveSymbolToTable(key, type, method, value);
    return isSuccessful;
}


/*
Recieves a line that contains a known action from commandNArgs:
1. Verify that the given command got N (reqArgsAmount) arguments
2. Counts the amount of decimal adress lines the command will take

Input: String command and amount of expected arguments it should get.
Output: number of lines the command will take if successful, -1 if there was an issue.
*/
int _verifyCommandArgs(char command[], int reqArgsAmount)
{
    int registryArgExists = 0,
        currArgsCount = 0,
        linesToAdd = 1;
    char *currArg, cmdName[MAX_LINE_LEN], args[MAX_LINE_LEN], *openSquare, *closeSquare;
    int length = sizeof(registriesNames) / sizeof(registriesNames[0]) - 1;

    /* Skip the command */
    sscanf(command, "%s %[^\n]", cmdName, args);

    /* Split the args */
    currArg = strtok(args, ", ");

    while (currArg) {
        currArgsCount += 1;

        /* check if it's registry argument */
        if (_stringArrayContains(registriesNames, length, currArg)) {
            if (!registryArgExists) {
                registryArgExists = 1;
                linesToAdd += 1;
            }
        }
        else {
            openSquare = strchr(currArg, '[');
            closeSquare = strchr(currArg, ']');

            /* check if it's a fixed index arg */
            if (openSquare && closeSquare && openSquare > closeSquare) {
                printf("Error in line %d: command %s got invalid argument name %s.\n", currDecimalAddr, cmdName, currArg);
                return -1;
            }
            else if (openSquare && closeSquare && openSquare < closeSquare)
                linesToAdd += 2;
            else {
                linesToAdd += 1;
            }
        }
        /* Get the next arg */
        currArg = strtok(NULL, ", ");
    }

    /* We got too many or not enough args */
    if (currArgsCount != reqArgsAmount) {
        printf("Error in line %d: command %s got %d args but expected %d.\n", currDecimalAddr, cmdName, currArgsCount, reqArgsAmount);
        return -1;
    }
    return linesToAdd;
}


/*
Accepts a line which we know contain code.
Checks if it has a label that we should save, validates the command arguments amount and
updates the global decimal adress.

Input: String line and the lineType which tells us the amount of args the command in it accepts.
Output: 1 if there is an issue, 0 otherwise.
*/
int _handleCodeLine(char line[], enum lineType type)
{
    int linesAmount, isSuccessful;
    char *label, *tempLine;

    /* copy the line to a temp field to not edit org line and search for label */
    label = _findLabel(line);
    tempLine = (char *) malloc(strlen(line)+1);
    strcpy(tempLine, line);
    strtok(tempLine, LABEL_SIGN);

    /* valid label was found, save it */
    if (label) {
        isSuccessful = _saveSymbolToTable(label, code, relocatable, currDecimalAddr);
        if (isSuccessful == 1) /* program failed */
            return isSuccessful;
        tempLine = strtok(NULL, LABEL_SIGN); /* tempLine currently holds the label too, move the next part of line to it */
    }

    /* update the decimal address and varify the command is valid */
    if (type == code0)
        linesAmount = _verifyCommandArgs(tempLine, 0);
    else if (type == code1)
        linesAmount = _verifyCommandArgs(tempLine, 1);
    else /* type == code2 */
        linesAmount = _verifyCommandArgs(tempLine, 2);

    if (linesAmount == -1) /* command invalid */
        return 1;

    currDecimalAddr += linesAmount;
    instructionsSectionLen += linesAmount;
    return 0;
}


/*
Commiting the first scan which is responsible on:
1. Going over the file generated by the Macro Scan
2. Creates the Symbols Table
3. Writes header to the Object file, containing:
   - the length of the instructions section
   - the length of the data section
4. Generates the Entry file
*/
int firstScan(char filename[])
{
    /* intializing variables, open and create a pointer to the needed file */
    int isSuccessful = 0;
    char line [MAX_LINE_LEN], *fullFileName, header[MAX_LINE_LEN];
    FILE *file;
    enum lineType currLineType;
    fullFileName = (char *) malloc(strlen(filename)+strlen(READ_FILE_TYPE)+1);
    sprintf(fullFileName, "%s%s", filename, READ_FILE_TYPE);
    file = fopen(fullFileName, "r");

    /* if failed to open the file, throws error */
    if (!file) {
        printf("Error: failed to open file %s\n" , fullFileName);
        return 1;
    }

    /* reading lines from the file */
    while (fgets(line, MAX_LINE_LEN, file)) {
        /* finding if the line is code, data or line we should ignore (comment and empty lines) */
        currLineType = _findInstructionType(line);

        if (currLineType == code0 || currLineType == code1 || currLineType == code2)
            isSuccessful = _handleCodeLine(line, currLineType);
        else if (currLineType == dataLine)
            isSuccessful = _handleDataLine(line);
           
        if (isSuccessful == 1)  /* program failed */
            return isSuccessful;
    }
    fclose(file);

    /* write Object file header */
    sprintf(header, "\t%d %d", instructionsSectionLen, dataSectionLen);
    writeToObjectFile(filename, header); 
    
    /* generating the entry file */
    generateEntryFile(filename);

    return isSuccessful;
}
