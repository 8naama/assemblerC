#include "dataStructures.h"
#include "filesGenerator.h"
#include "global.h"
#include "firstScan.h"


/* Initialize variables relevant to the first scan */
int currDecimalAddr = 100;  /* the decimal addresses counter */
struct Symbol *symbolTableNext;  /* pointer to mark place at the table */

/* variables for the Object file's header  */
int instructionsSectionLen = 0;
int dataSectionLen = 0;


/*
Recieves information for a new symbol to save, make sure it doesn't exist yet and saves it.
Since .entry and .extern are separate from a Symbol value and type definitions, handles updating the
Symbol's method or it's value and type accordingly.

Input: the new symbol info; name ,type (data or code), method (external, entry, relocatable), it's value and if it was defined already (0 for entry\extern, 1 otherwise).
Output: 0 if saved successfuly, 1 otherwise.
*/
int _saveSymbolToTable(char name[], enum SymbolType type, enum SymbolUpdateMethod method, int value, int isDefined) {
    Symbol *alreadyExists, *newItem;
    enum lineType isNameValid;

    alreadyExists = findInSymbolsTable(name);

    /* new Symbol */
    if (!alreadyExists) {

        /* Validate the new symbol's name*/
        isNameValid = findInstructionType(name);
        if (isRegistry(name) || isNameValid != none) {
            printf("Error: Symbol name '%s' is invalid.\nPlease make sure to name your symbol in a unique name that is not a registry or a saved instruction name.\n", name);
            return 1;
        }

        if (!symbolTableHead) {  /* The first item in the table */
            symbolTableHead = (struct Symbol *)malloc(sizeof(enum SymbolUpdateMethod) * sizeof(enum SymbolType) * sizeof(struct Symbol));

            if (!symbolTableHead) {
                printf("Error: Failed to allocate memory for Symbol %s\n", name);
                return 1;
            }

            strcpy(symbolTableHead->name, name);
            symbolTableHead->type = type;
            symbolTableHead->method = method;
            symbolTableHead->value = value;
            symbolTableHead->isDefinedAlready = isDefined;
            symbolTableHead->next = NULL;
            
            symbolTableNext = symbolTableHead;
        }
        else {  /* new item, not the first one */
            newItem = (Symbol*) malloc(sizeof(Symbol));
 
            if (!newItem) {
                printf("Error: Failed to allocate memory for Symbol %s\n", name);
                return 1;
            }

            strcpy(newItem->name, name);
            newItem->type = type;
            newItem->method = method;
            newItem->value = value;
            newItem->isDefinedAlready = isDefined;
            newItem->next = NULL;

            symbolTableNext->next = newItem;
            symbolTableNext = newItem;
        }
    }
    else {  /* symbol with given name exists in the table, 2 valid options below */

        /* Symbol's value and type saved already, now update that it's .entry or .extern */
        if (isDefined == 0 && alreadyExists->isDefinedAlready == 1)
            alreadyExists->method = method;
        /* Symbol saved as .entry or .extern, now update the value and line type*/
        else if (isDefined == 1 && alreadyExists->isDefinedAlready == 0) {
            alreadyExists->type = type;
            alreadyExists->value = value;
        }
        else {
            printf("Error: symbol %s already exists in symbol table and cannot be defined again.\n", name);
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

    if (!tempLine) {
        printf("Error: Failed to search for label due to memory allocation fail.\n");
        exit(1);
    }

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
    int value = 0,
        isDefined = 0,
        commaCount = 1,
        isSuccessful;
    char key[MAX_LABEL_NAME_LEN], currAction[7], *string, *ptr;
    enum SymbolType type = data;
    enum SymbolUpdateMethod method = relocatable;

    /* get needed variables from each action type */
    if (startsWith(line, ".define")) {
        sscanf(line, ".define %s = %d", key, &value);
        type = mdefine;
        isDefined = 1;
    }
    else if (startsWith(line, ".entry")) {
        sscanf(line, ".entry %s", key);
        method = entry;
    }
    else if (startsWith(line, ".extern")) {
        sscanf(line, ".extern %s", key);
        method = external;
    }
    else {
        /* initialize the symbol value */  
        value  = currDecimalAddr;
        isDefined = 1;

        /* check if .data or .string */  
        sscanf(line, "%[^:]: .%s", key, currAction);
        string = (char *) malloc(MAX_LINE_LEN-strlen(key)+1);

        if (!string) {
            printf("Error: failed to check data during first scan.\n");
            exit(1);
        }

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
    isSuccessful = _saveSymbolToTable(key, type, method, value, isDefined);
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

    /* Skip the command */
    sscanf(command, "%s %[^\n]", cmdName, args);

    /* Split the args */
    currArg = strtok(args, ", ");

    while (currArg) {
        currArgsCount += 1;

        /* check if it's registry argument */
        if (isRegistry(currArg)) {
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

    if (!tempLine) {
        printf("Error: Failed to allocate memory during first scan.\n");
        exit(1);
    }

    strcpy(tempLine, line);
    strtok(tempLine, LABEL_SIGN);

    /* valid label was found, save it */
    if (label) {
        isSuccessful = _saveSymbolToTable(label, code, relocatable, currDecimalAddr, 1);
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

    /* opening the file */
    fullFileName = (char *) malloc(strlen(filename)+strlen(MACRO_OUTPUT_FILE_TYPE)+1);
    if (!fullFileName) {
        printf("Error: Failed to allocate memory during first scan.\n");
        exit(1);
    }

    sprintf(fullFileName, "%s%s", filename, MACRO_OUTPUT_FILE_TYPE);
    file = fopen(fullFileName, "r");

    /* if failed to open the file, throws error */
    if (!file) {
        printf("Error: failed to open file %s\n" , fullFileName);
        return 1;
    }

    /* reading lines from the file */
    while (fgets(line, MAX_LINE_LEN, file)) {
        /* finding if the line is code, data or line we should ignore (comment and empty lines) */
        currLineType = findInstructionType(line);

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

    free(fullFileName);
    return isSuccessful;
}

