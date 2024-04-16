#include "macro.h"
#include "global.h"


/*
Checks whether it is the beginning of a macro or the end of a macro

Input: String line
Output: 1 if start of a macro, -1 if end of macro, 0 otherwise
*/
int _isMacroDefinition(char line[])
{
    int index = 0 , mIndex = 0 ;
    char mcr [MAX_LINE_LEN];

    memset(mcr , '\0' , MAX_LINE_LEN);

    while(isspace(line[index]))
        index ++;
    while (line[index] != '\n' && !isspace(line[index])) {
        mcr[mIndex] = line[index];
        mIndex++;
        index++;
    }
    if (!strcmp(mcr, "mcr"))
        return 1;
    if (!strcmp(mcr, "endmcr"))
        return -1;
    return 0;
}


/*
Enter the macro name in the macros table

Input: Pointer to a Macro struct and String line
Output: none
*/
void _insertMacroNameToTable(struct Macro *pMcr, char line[])
{
    char mName[MAX_LINE_LEN];
    enum lineType isNameValid;

    sscanf(line, "mcr %s\n", mName);

    /* Validate macro name */
    isNameValid = findInstructionType(mName);
    if (isNameValid != none || isRegistry(mName)) {
        printf("Error: Macro name '%s' is invalid.\nPlease make sure to name your Macros in a unique name that is not a saved instruction name\n", mName);
        exit(1);
    }

    strcpy(pMcr->name,mName);
}



/*
Inserts the macro contents into the macros table

Input: Pointer to a Macro struct and a file pointer
Output: none
*/
void _insertMacroContentToTable(struct Macro *pMcr, FILE *fp)
{
    char line [MAX_LINE_LEN];
    char mContent [MAX_LINE_LEN];

    memset(line , '\0' , MAX_LINE_LEN);
    memset(mContent , '\0' , MAX_LINE_LEN);
    fgets(line, MAX_LINE_LEN, fp);

    while(_isMacroDefinition(line) != -1) {
        strncat(mContent, line , MAX_LINE_LEN);   
        fgets(line, MAX_LINE_LEN, fp);
    }
    strcpy(pMcr->content,mContent);
}



/*
Performing the first pass on the file (inserting the macros into the macro table,
copying the corresponding rows from the table to the file, etc.)

Input: filename and pointer to the Macro table head
Output: 1 if failed, 0 if suceeded
*/
int _readFile(char filename[] ,struct Macro *mHead)
{
    char line [MAX_LINE_LEN];
    FILE *fpr;
    memset(line , '\0' , MAX_LINE_LEN);
    fpr = fopen(filename,"r");

    if(fpr == NULL) {
        printf("Error: failed to open file: %s\n" , filename);  
        return 1;
    }

    while(fgets(line, MAX_LINE_LEN, fpr)) {
        struct  Macro* temp = NULL;
        temp = (struct Macro*)malloc(sizeof(struct Macro));

        /* start of macro */
        if(_isMacroDefinition(line) == 1) { 
            _insertMacroNameToTable(temp , line);
            _insertMacroContentToTable(temp , fpr);
            mHead -> next = temp ;
            mHead = temp;
        }
    }
    return 0;
}


/*
Copy the contents of the corresponding macro to the file from the table, if it is a macro command

Input: string line, file pointer and Macro table head tail
Output: 1 if given line is a macro command, 0 otherwise
*/
int _isMacroCommand(char line[], FILE *fpw,struct Macro *mTail)
{
    int index = 0, mIndex = 0;
    char mName [MAX_LINE_LEN];
    struct  Macro *temp = NULL;
    temp = (struct Macro*)malloc(sizeof(struct Macro));
    temp = mTail;
    memset(mName, '\0' , MAX_LINE_LEN);

    while(isspace(line[index]))
	index ++;
    while (!isspace(line[index]) && line[index] != '\n') {
	mName[mIndex] = line[index];
    	mIndex++;
    	index++;
    }
    while (temp != NULL) {
	if (!strcmp(temp->name , mName)) {
            fprintf(fpw, "%s", temp->content);
	    return 1;
        }
        temp = temp ->next;
    }
    return 0;
}


/*
Checks if the given line is macro or not and returns mFlag accordingly.

Input: output file pointer, Macro table tail and String line
Output: -1 if the line should be ignored, 1 if it's a macro call, 0 otherwise (regular line).
*/
int _checkLine(FILE *fpw, struct Macro *tail, char *line) {
    static int mFlag;
    int isMacroRelated;

    isMacroRelated = _isMacroDefinition(line);

    /* middle of macro definition >> ignore */
    if (mFlag == -1) {
        /* end of macro definition >> ignore this line but check the next */
        if (isMacroRelated == -1) {
            ++mFlag;
            return -1;
        }
        mFlag = -1;
    }
    /* start of macro */
    else if (isMacroRelated == 1)
    	mFlag = -1;
    /* known macro call */
    else if (_isMacroCommand(line, fpw, tail) == 1) 
    	mFlag = 1;
    /* regular line */
    else
        mFlag = 0;
    return mFlag;
}


/*
Writes lines from given inputFilename to outputFilename based on the checkLine() function result.

Input: String inputFilename and outputFilename, and a pointer to the macro table
Output: none
*/
void _writeFile(char inputFilename[], char outputFilename[], struct Macro *tail) {
    FILE *fpr, *fpw;
    char line[MAX_LINE_LEN];
    int mflag;

    memset(line, '\0', MAX_LINE_LEN);

    fpr = fopen(inputFilename, "r");
    fpw = fopen(outputFilename, "w");

    if (fpr == NULL)
	printf("Error: failed to open file: %s\n", inputFilename);

    while (fgets(line, MAX_LINE_LEN, fpr)) {
    mflag = _checkLine(fpw, tail, line);

    /* got line that should be written to the outputFilename */
    if (mflag == 0)
    	fprintf(fpw, "%s", line);
    }

    fclose(fpw);
    fclose(fpr);
}


/*
Commiting the macro spreading scan which finds all the macros in the file, replaces them and writes the new content to MACRO_OUTPUT_FILE_TYPE file that is defined in macro.h
*/
void spreadMacros(char filename[]) {
    int flag;
    char *readfilename, *writefilename;
    struct Macro *mTail = NULL;
    struct Macro *mHead = NULL;

    readfilename = (char *) malloc(strlen(filename)+4);
    writefilename = (char *) malloc(strlen(filename)+4);
    sprintf(readfilename, "%s%s", filename, ASSEMBLY_FILE_TYPE);
    sprintf(writefilename, "%s%s", filename, MACRO_OUTPUT_FILE_TYPE);

    mHead = (struct Macro*)malloc(sizeof(struct Macro));
    mTail = (struct Macro*)malloc(sizeof(struct Macro));
    mTail = mHead;

    flag = _readFile(readfilename, mHead);
    if (!flag) {
        _writeFile(readfilename, writefilename, mTail);
    }
    free(readfilename);
    free(writefilename);
}
