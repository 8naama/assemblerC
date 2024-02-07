#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100

struct Macro {
    char name[MAX];
    char content[MAX];
    struct Macro* next;
};
/*void addToMtable(struct Macro *head , char name[] , char content[]);
void printMacroTable(struct Macro *tail);*/
int isMacro(char line[]);
void insertName(struct Macro *pMcr, char line[]);
void insertContent(struct Macro *pMcr, FILE *fpr);
int isMacroCommand(char line[], FILE *fpw, struct Macro *mTail);
int readFile(int i, char *argv[], struct Macro *mHead);
void checkLine(FILE *fpw, struct Macro *tail, char *line);
void writeFile(int i, char *argv[],struct Macro *mTail);
