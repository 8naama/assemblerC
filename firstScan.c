#include "firstScan.h"
#include "dataStructures.h"


int currDecimalAddr = 100;  /* Initialize the decimal addresses counter */


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
	/* read the first word in the given line */
	char *word = strtok(line, " \t");

	/* continue reading words until we figure out the sentance type or reach it's end */
	while (word != NULL) {
		if (_startsWith(word, COMMENT_SIGN))
			return NULL;
		else if (_isData(word))
			return data;
		enum lineType codeType = _isCode(word)
		if (codeType)
			return codeType;

		/* read the next word in the given line */
        word = strtok(NULL, " \t");
    }
	return NULL;
}


struct Symbol *_saveLabelToTable(struct Symbol *pointer, char label[], enum SymbolType type, enum SymbolUpdateMethod method, int value) {
	// Need to make sure here if a Label already exists or not,
	// FOR EXAMPLE: .entry and .extern come before the value does 
	// so we need to keep just the SymbolUpdateMethod on the side until we get to the value

	// Maybe something like:
	// if exists in the table >> check SymbolUpdateMethod, if it's entry or external then don't keep the new val
	// OR something like:
	// keep side list with label names that already came up and flags for entry, extern and saved true/false
	// once we get new var we check if it exists already (saved) or not
	// if not saved - save it wih relevant details and add the name to the array
	// if saved - throw error
} // I want this to return pointer to our Symbols table


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


int _handleDataLine(char line[])
{
	// How do we handle data line:
	// 1. check if .define if so, keep in table + add 1 to decimalAddr
	// 2. if not, find if label
	// 3. if .entry or .external, turn flag on in the table
	// 4. count arguments to know how many lines will it take up to add to currDecimalAddr
	// Need to think if that's the best and if so create function that can search on the data structure 
	// and return pointer

	int linesAmount;
	char *label, *tempLine;

	label = _findLabel(line);

	if (label != NULL) {
		/* skip the label */
		strcpy(tempLine, line);
		strtok(tempLine, LABEL_SIGN);
		tempLine = strtok(NULL, LABEL_SIGN);

		// COLLECT INFORMATION HERE ABOUT the label value

		_saveLabelToTable(label, data, relocatable, value);
	}

	// CHECK HERE IF mdefine

	// CHECK HERE IF ENTRY / EXTERNAL


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
	int linesAmount;
	char *label, *tempLine;

	/* copy the line to a temp field to not edit org line and search for label */
	label = _findLabel(line);
	tempLine = (char *) malloc(strlen(line)+1);
	strcpy(tempLine, line);
	strtok(tempLine, LABEL_SIGN);

	/* valid label was found, save it */
	if (label != NULL) {
		_saveLabelToTable(label, code, relocatable, currDecimalAddr);
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
    if (fp == NULL) {
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
	    else if (lineType == data) {
	        isSuccessful = _handleDataLine(line);
	        if (isSuccessful == 1)  /* program failed */
	        	return isSuccessful;
	    }
    }
    fclose(file);
    return isSuccessful;
}
