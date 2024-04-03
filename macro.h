#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100
#define INPUT_FILE_TYPE ".as"
#define OUTPUT_FILE_TYPE ".am"


struct Macro {
    char name[MAX];
    char content[MAX];
    struct Macro* next;
};

void spreadMacros(char filename[]);
