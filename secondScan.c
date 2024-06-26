#include "secondScan.h"
#include "dataStructures.h"
#include "filesGenerator.h"
#include "global.h"


int currentAddr = 100;  /* the decimal addresses counter */


/*
Translates the given number 'n' to binary with numBits bits.

Input: number n and the amount of bits to output it's binary format
Output: n in binary in length numBits.
*/
char* _decimalToBinary(int n, int numBits) {
    char *p;
    int i, absN;
    p = (char*)malloc((numBits + 1) * sizeof(char));

    absN = abs(n);

    if (p == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }

    for (i = 0; i < numBits; i++) {
        p[i] = '0';
    }
    p[numBits] = '\0';

    i = numBits - 1;
    while (absN > 0 && i >= 0) {
        p[i] = absN % 2 + '0'; 
        absN = absN / 2;
        i--;
    }

    /* handle negative number */
    if (n < 0) {
        /* reverse bits */
        for (i = 0; i < numBits; i++) {
            p[i] = (p[i] == '0') ? '1' : '0';
        }
        /* add 1 */
        for (i = numBits - 1; i >= 0; i--) {
            if (p[i] == '0') {
                p[i] = '1';
                break;
            }
            else {
                p[i] = '0';
            }
        }
    }
    return p;
}


/* 
Returns the line without the label part if exists.
If doesn't exists return the original line.

Input: String line
Output: The line without label, if no label found, returns the original line
*/
char* _skipLabelAndSpaces(char line[])
{
    char *tempLine;
    int i = 0;

    /* copy the line to a temp field to not edit org line and search for label */
    tempLine = (char *) malloc(strlen(line)+1);

    if (!tempLine) {
        printf("Error: Failed to search for label due to memory allocation fail.\n");
        exit(1);
    }

    strcpy(tempLine, line);
    strtok(tempLine, LABEL_SIGN);

    /* found label, skip it */
    if (strcmp(tempLine, line) != 0) {
        tempLine = strtok(NULL, LABEL_SIGN);
        tempLine++;
        return tempLine;
    }
    while (isspace(line[i++]))
        line++;
    free(tempLine);
    return line;
}


/*
Generate binary words for data lines (.string and .data).

Input: the name of the filename to output the words to and the line to translate.
Output: none
*/
void _generateDataWords(char filename[], char line[])
{
    char action[7], args[MAX_LINE_LEN], arg[MAX_LABEL_NAME_LEN], newWord[15] = "00000000000000", *ptr;
    int i, val;
    Symbol *isSymbol;
    sscanf(line, ".%s %[^\n]", action, args);

    if (strcmp(action, "string") == 0) {
        for (i = 1; i < strlen(args)-1; i++) {
            if (isalpha(args[i])) {
                strncpy(newWord, _decimalToBinary((int)args[i], BINARY_WORD_SIZE), BINARY_WORD_SIZE);
                addBinaryWordInObjectFile(filename, currentAddr, newWord);
                currentAddr++;
            }
        }
        /* create word for '\0' */
        strncpy(newWord, _decimalToBinary(0, BINARY_WORD_SIZE), BINARY_WORD_SIZE);
        addBinaryWordInObjectFile(filename, currentAddr, newWord);
        currentAddr++;
    }
    else { /* action == data */
        ptr = args;
        while (sscanf(ptr, "%[^, ], ", arg) == 1) {
            isSymbol = findInSymbolsTable(arg);

            /* make binary word for curr arg */
            if (isSymbol)
                val = isSymbol->value;
            else
                val = atoi(arg);

            strncpy(newWord, _decimalToBinary(val, BINARY_WORD_SIZE), BINARY_WORD_SIZE);
            addBinaryWordInObjectFile(filename, currentAddr, newWord);
            currentAddr++;

            /* move to the next arg */
            ptr = strchr(ptr, ',');

            if (ptr == NULL)
                break;
            ptr+=2; /* skip the comma and space */
        }

    }
    
}


/*
Returns the op code of the given operation.

Input: operation name
Output: the operation code
*/
char *_getOpCode(char op[])
{
    char *opBin;

    opBin = (char *) malloc(5);

    if(strcmp(op,"mov") == 0)
        strcpy(opBin, "0000");
    else if(!strcmp(op,"cmp"))
        strcpy(opBin,"0001");
    else if(!strcmp(op, "add"))
        strcpy(opBin,"0010");
    else if(!strcmp(op, "sub"))
        strcpy(opBin,"0011");
    else if(!strcmp(op, "not"))
	strcpy(opBin,"0100");
    else if(!strcmp(op, "clr"))
	strcpy(opBin,"0101");
    else if(!strcmp(op, "lea"))
	strcpy(opBin,"0110");
    else if(!strcmp(op, "inc"))
	strcpy(opBin,"0111");
    else if(!strcmp(op, "dec"))
	strcpy(opBin,"1000");
    else if(!strcmp(op, "jmp"))
	strcpy(opBin,"1001");
    else if(!strcmp(op, "bne"))
	strcpy(opBin,"1010");
    else if(!strcmp(op, "red"))
	strcpy(opBin,"1011");
    else if(!strcmp(op, "prn"))
	strcpy(opBin,"1100");
    else if(!strcmp(op, "jsr"))
	strcpy(opBin,"1101");
    else if(!strcmp(op, "rts"))
	strcpy(opBin,"1110");
    else /*op == hlt */
	strcpy(opBin,"1111");
    return opBin;
}


/*
Recieves an argument and returns it's address type (immidiate, direct, fixed index, register)

Input: argument name
Output: the argument address type.
*/
enum argType _findArgumentType(char arg[])
{
    if (strlen(arg) == 0)
        return noArg;
    if (startsWith(arg, IMMIDIATE_SIGN))
        return immediate;
    else if(findInSymbolsTable(arg))
        return direct;
    else if(isRegistry(arg))
        return directRegister;
    return fixedIndex;
}


/*
Returns the binary code of the given argument address type.

Input: argument adress type (immidiate, direct, fixed index, register)
Output: the argument address type binary code
*/
char *_getArgumentTypeBinary(enum argType type)
{
    char *result;

    result = (char *) malloc(3);
    if (type == immediate || type == noArg)
        strcpy(result, IMMIDIATE_BINARY);
    else if (type == direct)
        strcpy(result, DIRECT_BINARY);
    else if (type == fixedIndex)
        strcpy(result, FIXED_INDEX_BINARY);
    else /* type == directRegister */
        strcpy(result, DIRECT_REGISTER_BINARY);
    return result;
}


/*
Returns the number of the given register

Input: register name (r3, r8, r5..)
Output the register number (3, 8, 5..)
*/
int _getRegistryNum(char arg[])
{
    int num;
    sscanf(arg, "r%d", &num);
    return num;
}


/*
Generates the binary word of operation where both the source and the destenation were registers.

Input: source and destenation arguments names and the filename to write the words at.
Output: none
*/
void _generateMultiRegistryWord(char filename[], char srcArg[], char destArg[])
{
    char newWord[15] = "000000xxxxxx00";
    int srcNum, destNum;
    srcNum = _getRegistryNum(srcArg);
    destNum = _getRegistryNum(destArg);

    strncpy(newWord+NEXT_WORD_REG_SRC_INDEX, _decimalToBinary(srcNum, 3), 3);
    strncpy(newWord+NEXT_WORD_REG_DEST_INDEX, _decimalToBinary(destNum, 3), 3);
    addBinaryWordInObjectFile(filename, currentAddr, newWord);
    currentAddr++;
}


/*
Generates binary words for operation that had a register as a source or destenation.

Input: the argument name, and srcOrdest (0 for source, else destenation) and the filename to output the binary words to.
Output: none
*/
void _generateNextRegWord(char filename[], char arg[], int srcOrdest)
{
    char newWord[15] = "000000xxxxxx00";
    int regNum, empty = 0;
    regNum = _getRegistryNum(arg);

    if (srcOrdest == 0) {  /* src */
        strncpy(newWord+NEXT_WORD_REG_SRC_INDEX, _decimalToBinary(regNum, 3), 3);
        strncpy(newWord+NEXT_WORD_REG_DEST_INDEX, _decimalToBinary(empty, 3), 3);
    }
    else {  /* dest */
        strncpy(newWord+NEXT_WORD_REG_DEST_INDEX, _decimalToBinary(regNum, 3), 3);
        strncpy(newWord+NEXT_WORD_REG_SRC_INDEX, _decimalToBinary(empty, 3), 3);
    }
    addBinaryWordInObjectFile(filename, currentAddr, newWord);
    currentAddr++;
}


/*
Generates binary word for operation that had argument with direc address.

Input: argument name and the filename to output the binary word to.
Output: none.
*/
void _generateNextDirectWord(char filename[], char arg[])
{
    char newWord[15] = "xxxxxxxxxxxxxx";
    int val;
    Symbol *isSymbol;

    isSymbol = findInSymbolsTable(arg);

    if (!isSymbol) {
        printf("Error: Failed to find used argument %s.\n", arg);
        exit(1);
    }
    
    if (isSymbol && isSymbol->method == external) {
        strncpy(newWord+ARG_TYPE_START_INDEX, DIRECT_BINARY, strlen(DIRECT_BINARY));
        writeToExternalFile(filename, arg, currentAddr);
    }
    else
        strncpy(newWord+ARG_TYPE_START_INDEX, FIXED_INDEX_BINARY, strlen(DIRECT_BINARY));
    val = isSymbol->value;
    strncpy(newWord, _decimalToBinary(val, 12), 12);
    addBinaryWordInObjectFile(filename, currentAddr, newWord);
    currentAddr++;
}


/*
Generates binary word for operation that had an argument with index address.

Input: argument name and the filename to output the binary words to.
Output: none.
*/
void _generateNextIndexWord(char filename[], char arg[])
{
    char newWord[15] = "xxxxxxxxxxxx00";
    int val;
    Symbol *isSymbol;

    isSymbol = findInSymbolsTable(arg);

    if (isSymbol)
        val = isSymbol->value;
    else
        val = atoi(arg);
    strncpy(newWord, _decimalToBinary(val, 12), 12);
    addBinaryWordInObjectFile(filename, currentAddr, newWord);
    currentAddr++;
}


/*
Generates binary word for operation that had an argument with immidiate address.

Input: argument name and the filename to output the binary words to.
Output: none.
*/
void _generateNextImmWord(char filename[], char arg[])
{
    char val[MAX_LABEL_NAME_LEN];

    sscanf(arg, "#%[^\n]", val);
    _generateNextIndexWord(filename, val);
}


/*
Manages which functions to call to generate binary words per the given argument and it's adress type.

Input: argument name and address type, srcOrdest (0 for source, else destenation) and the filename to output the words into.
Output: none
*/
void _generateNextWordPerArgType(char filename[], char arg[], enum argType type, int srcOrdest)
{
    char arr[MAX_LABEL_NAME_LEN], ind[MAX_LABEL_NAME_LEN]; 

    if (type == immediate) 
        _generateNextImmWord(filename, arg);
    else if (type == direct)
        _generateNextDirectWord(filename, arg);
    else if (type == fixedIndex) {
        sscanf(arg, "%[^[][%[^]]]", arr, ind);
        _generateNextDirectWord(filename, arr);
        _generateNextIndexWord(filename, ind);
    }
    else /* type == directRegister */
        _generateNextRegWord(filename, arg, srcOrdest);
}


/*
Manages the creation of the binary words of code instructions, after the first word;
Sends the given arguments to functions which generates binary words based on the given source and destenation arguments types.

Input: source and destenation argument names and address types, as well as filename to output the files for.
Output: none
*/
void _generateNextCodeWords(char filename[], char srcArg[], enum argType srcType, char destArg[], enum argType destType)
{
    if (srcType == directRegister && destType == directRegister) {
        _generateMultiRegistryWord(filename, srcArg, destArg);
    }
    else {
        /* generate source word\s */
        if (srcType != noArg)
            _generateNextWordPerArgType(filename, srcArg, srcType, 0);

        /* generate destenation word\s */
        if (destType != noArg)
            _generateNextWordPerArgType(filename, destArg, destType, 1);
    }
}


/*
1. Parses the line to extrac from it the command, the source and the destentation (if they exist)
2. If exists, check the source and destentation addressing type
3. Generates the first binary word for code instructions and outputs to the object file with the given file's name.
4. Call function  _generateNextCodeWords to generate the rest of the binary words.

Input: instruction line and it's type (code0, code1, code2, data) and the filename to output the binary words into at the end.
Output: none
*/
void _generateCodeWords(char filename[], char line[], enum lineType currLineType)
{
    char firstBinaryWord[15] = "0000xxxxxxxx00",
         command[4] = "\0",
         src[MAX_LINE_LEN] = "\0",
         dest[MAX_LINE_LEN] = "\0",
         opCode[5], srcBinaryCode[3], destBinaryCode[3];
    enum argType destType, srcType;

    /* Generate the first word */
    if (currLineType == code0) {
        sscanf(line, "%[^ \n]", command);
    }
    else if (currLineType == code1) {
        sscanf(line, "%4s %[^ \n]", command, dest);
    }
    else {  /* currLineType == code2 */
        sscanf(line, "%4s %[^,], %[^ \n]", command, src, dest);
    }

    /* Add opCode to the first word */
    strcpy(opCode, _getOpCode(command));
    strncpy(firstBinaryWord+OP_CODE_START_INDEX, opCode, strlen(opCode));

    /* Find the dest and src types and update the first word */
    destType = _findArgumentType(dest);
    strcpy(destBinaryCode, _getArgumentTypeBinary(destType));

    srcType = _findArgumentType(src);
    strcpy(srcBinaryCode, _getArgumentTypeBinary(srcType));

    strncpy(firstBinaryWord+FIRST_WORD_SRC_START_INDEX, srcBinaryCode, strlen(srcBinaryCode));
    strncpy(firstBinaryWord+FIRST_WORD_DST_START_INDEX, destBinaryCode, strlen(destBinaryCode));

    /* Add the word to the binary file */
    addBinaryWordInObjectFile(filename, currentAddr, firstBinaryWord);
    currentAddr++;

    /* Generate next words */
    _generateNextCodeWords(filename, src, srcType, dest, destType);
}


/* 
Checks the given line's type and sends it to relevant function that would generate the binary word.

Input: the line that was read and the filename to output the words into
Output: none
*/
void _generateBinaryWords(char filename[], char line[]) {
    enum lineType currLineType;
    currLineType = findInstructionType(line);

    if (currLineType == dataLine)
        _generateDataWords(filename, line);
    else
        _generateCodeWords(filename, line, currLineType);
}


/* 
Performs the second scan run:
1. Generates the external file
2. Translates the instructions to binary words and add to the Object file.
*/
void secondScan(char filename[])
{
    char line [MAX_LINE_LEN], *fullFileName, *lineNoLabel;
    FILE *file;

    /* opening the file */
    fullFileName = (char *) malloc(strlen(filename)+strlen(MACRO_OUTPUT_FILE_TYPE)+1);
    if (!fullFileName) {
        printf("Error: Failed to allocate memory during second scan.\n");
        exit(1);
    }

    sprintf(fullFileName, "%s%s", filename, MACRO_OUTPUT_FILE_TYPE);
    file = fopen(fullFileName, "r");

    /* if failed to open the file, throws error */
    if (!file) {
        printf("Error: failed to open file %s\n" , fullFileName);
        exit(1);
    }

    /* reading lines from the file */
    while (fgets(line, MAX_LINE_LEN, file)) {
        if (startsWith(line, COMMENT_SIGN) || startsWith(line, ".define") || startsWith(line, ".extern") || startsWith(line, ".entry"))
            continue;
    
    lineNoLabel = _skipLabelAndSpaces(line);
    _generateBinaryWords(filename, lineNoLabel);
    }
}
