#include "dataStructures.h"
#include "firstScan.h"

// We need 2 pointers for the data structure. a global one, called head, which will be used by both scans,
// and a local one next, that we will use to monitor the progress of the table and add the next item to it's 
// next each time 


int currDecimalAddr = 100;  /* Initialize the decimal addresses counter */
struct Symbol *symbolTableNext;


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
Returns if the given word is in the definitionAndDirective array.

Input: string word.
Output: 1 if the word means the line is data line, 0 otherwise
*/
int _isData(char word[]) 
{
	int length = sizeof(definitionAndDirective) / sizeof(definitionAndDirective[0]) - 1;
	return _stringArrayContains(definitionAndDirective, length, word);
}


/*
Returns if the given word is a known code word and it's type:
- code0: no args
- code1: 1 arg
- code2: 2 args

Input: string word.
Output: code type if the word is code, NULL otherwise
*/
enum lineType _isCode(char word[]) 
{
	int length0 = sizeof(commandNoArgs) / sizeof(commandNoArgs[0]) - 1,
		length1 = sizeof(commandOneArgs) / sizeof(commandOneArgs[0]) - 1,
		length2 = sizeof(commandTwoArgs) / sizeof(commandTwoArgs[0]) - 1;

	if (_stringArrayContains(commandNoArgs, length0, word))
		return code0;
	else if (_stringArrayContains(commandOneArgs, length1, word))
		return code1;
	else if (_stringArrayContains(commandTwoArgs, length2, word))
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
	char *tempLine;

	/* copy the line to a temp field to not edit org line and search for label */
	tempLine = (char *) malloc(strlen(line)+1);
	strcpy(tempLine, line);

	/* read the first word in the given line */
	char *word = strtok(tempLine, " \t");

	/* continue reading words until we figure out the sentance type or reach it's end */
	while (word != NULL) {
		if (_startsWith(word, COMMENT_SIGN)) {
			free(tempLine);
			return none;
		}
		else if (_isData(word)) {
			free(tempLine);
			return dataLine;
		}
		enum lineType codeType = _isCode(word);
		if (codeType != none) {
			free(tempLine);
			return codeType;
		}

		/* read the next word in the given line */
        word = strtok(NULL, " \t");
    }
    free(tempLine);
	return none;
}


/*
Recieves information for a new symbol to save, make sure it doesn't exist yet and saves it.
Since .entry and .extern are separate from a Symbol value and type definitions, handles updating the
Symbol's method or it's value and type accordingly.

Input: the new symbol info; name ,type (data or code), method (external, entry, relocatable) and it's value.
Output: 0 if saved successfuly, 1 otherwise.
*/
int *_saveSymbolToTable(char name[], enum SymbolType type, enum SymbolUpdateMethod method, int value) {
	Symbol *alreadyExists;

	alreadyExists = findInSymbolsTable(name); /* TODO: function to create in dataStructures.c */

	/* new Symbol */
	if (alreadyExists == NULL) {
		Symbol newItem = {name, type, method, value};

		if (symbolTableHead == NULL) /* The first item in the table */
			symbolTableHead = &newItem;
		else 
			symbolTableNext->next = &newItem;
		symbolTableNext = &newItem
		
	}
	else {
		/* we already saved the symbol's value and type, now we update that it's .entry or .extern */
		if ((method == entry || method == external) && alreadyExists->method == relocatable)
			alreadyExists->method = method;
		/* we already saved that the symbol is .entry or .extern, now we update the value and line type*/
		else if (alreadyExists->method == entry || alreadyExists->method == external) {
			alreadyExists->type = type;
			alreadyExists->value = value;
		}
		else {
			printf("error: symbol %s already exists in symbol table and cannot be defined again.", name);
			return 1;
		}
	}
	return 0;
}


char *_findLabel(char line[])
{
	char *label, *tempLine;

	/* copy the line to a temp field to not edit org line and search for label */
	tempLine = (char *) malloc(strlen(line)+1);
	strcpy(tempLine, line);
	label = strtok(tempLine, LABEL_SIGN);
	free(tempLine);

	/* label was found, verify length limitation */
	if (strcmp(label, line) != 0 && strlen(label) > MAX_LABEL_NAME_LEN) {
		printf("error: label %s is passing the allowed %d characters limit.\n", label, MAX_LABEL_NAME_LEN);
		exit(1);
		// return NULL;
	}
	/* valid label was found, return it */
	if (strcmp(label, line) != 0) 
		return label;
	return NULL;
}


int _handledataLine(char line[])
{
	// How do we handle data line:
	// 1. check if .define if so, keep in table
	// 2. if .entry or .external, turn flag on in the table
	// 3. if not, find if label
	// 4. if .data or .string, count arguments to know how many lines will it add to currDecimalAddr

	/* initializing variables */
	int linesAmount = 0, 
		value = -1, 
		isSuccessful;
	char *tempLine, key[MAX_LABEL_NAME_LEN];
	enum SymbolType type = data;
	enum SymbolUpdateMethod method = relocatable;

	if (_startsWith(line, ".define")) {
		sscanf(s, ".define %s = %d", key, &value);
		type = mdefine;
	}
	else if (_startsWith(line, ".entry")) {
		sscanf(s, ".entry %s", key);
		method = entry;
	}
	else if (_startsWith(line, ".extern")) {
		sscanf(s, ".extern %s", key);
		method = external;
	}
	else { /* .data or .string */  
		key = *_findLabel(line);
		value  = currDecimalAddr;

		// for .string, will need to understand the legth of the string inside the "" = X
	    // for .data, will need to count the amount of arguments it got = X
		// so will do value = currDecimalAddr and then currDecimalAddr += X
	}

	isSuccessful = _saveSymbolToTable(key, type, method, value);
	return isSuccessful
}


/*
1. Verify that the given command got reqArgsAmount arguments
2. Counts the amount of decimal adress lines the command will take

Input: String command and amount of expected arguments it should get.
Output: number of lines the command will take if successful, -1 if there was an issue.
*/
int _verifyCommandArgs(char command[], int reqArgsAmount)
{
	int registryArgExists = 0,
		currArgsCount = 0,
		linesToAdd = 1;
	char *currArg, *cmdName, *openSquare, *closeSquare;
	int length = sizeof(registriesNames) / sizeof(registriesNames[0]) - 1;

	/* Skip the command */
	cmdName = strtok(command, " \t");
	command = strtok(NULL, " \t");

	/* Split the args */
	currArg = strtok(command, ",");

	while (currArg != NULL) {
		currArgsCount += 1;

		/* check if it's registry argument */
		if (_stringArrayContains(registriesNames, length, currArg) && !registryArgExists) {
			registryArgExists = 1;
			linesToAdd += 1;
		}
		else {
			openSquare = strchr(currArg, '[');
			closeSquare = strchr(currArg, ']');

			/* check if it's a fixed index arg */
			if (openSquare && closeSquare && openSquare > closeSquare) {
				printf("error in line %d: command %s got invalid argument name %s.\n", currDecimalAddr, cmdName, currArg); 
				return -1;
			}
			else if (openSquare && closeSquare && openSquare < closeSquare)
				linesToAdd += 2;
			else {
				linesToAdd += 1;
			}
		}
		/* Get the next arg */
		currArg = strtok(NULL, ",");
	}

	/* We got too many or not enough args */
	if (currArgsCount != reqArgsAmount) {
		printf("error in line %d: command %s got %d args but expected %d.\n", currDecimalAddr, cmdName, currArgsCount, reqArgsAmount); 
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
	if (label != NULL) {
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

	if (linesAmount = -1) /* command invalid */
	return 1;

	currDecimalAddr += linesAmount;
	return 0;
}


int firstScan(char filename[]) 
{
	/* intializing variables, open and create a pointer to the needed file */
	int isSuccessful = 0;
	char line [MAX];
	FILE *file;
    sprintf(filename, "%s%s", filename, READ_FILE_TYPE);
    file = fopen(filename, "r");

    /* if failed to open the file, throws error */
    if (file == NULL) {
    	printf("error: the file: %s can't open\n" , filename); 
	    return 1;
    }

    /* reading lines from the file */
    while (fgets(line, MAX_LINE_LEN, file)) {
    	/* finding if the line is code, data or line we should ignore (comment and empty lines) */
    	enum lineType lineType = _findInstructionType(line);

	    if (lineType == code0 || lineType == code1 || lineType == code2) {
	        isSuccessful = _handleCodeLine(line, lineType);
	        if (isSuccessful == 1)  /* program failed */
	        	return isSuccessful;
	    }
	    else if (lineType == dataLine) {
	        isSuccessful = _handledataLine(line);
	        if (isSuccessful == 1)  /* program failed */
	        	return isSuccessful;
	    }
    }
    fclose(file);
    return isSuccessful;
}
