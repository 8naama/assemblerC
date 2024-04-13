#include "global.h"


char *commandTwoArgs[5] = {"mov", "cmp", "add", "sub", "lea"};
char *commandOneArgs[9] = {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
char *commandNoArgs[2] = {"rts", "hlt"};
char *definitionAndDirective[5] = {".data", ".string", ".entry", ".extern", ".define"};
char *registriesNames[8] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};


/*
If given word starts with given substring, return 1 otherwise return 0.

Input: word to check if starts with given substring
Output: 1 if word starts with substring, 0 otherwise
*/
int startsWith(char *word, char *substring)
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
enum lineType findInstructionType(char line[])
{
    enum lineType codeType;

    if (startsWith(line, COMMENT_SIGN))
        return none;
    else if (_isData(line))
        return dataLine;
    codeType = _isCode(line);
        return codeType;
    return none;
}


/*
Returns 1 if the given argument is a registry, 0 otherwise.

Input:
Output:
*/
int isRegistry(char arg[])
{
    int length = sizeof(registriesNames) / sizeof(registriesNames[0]) - 1;
    if (_stringArrayContains(registriesNames, length, arg))
        return 1;
    return 0;
}
