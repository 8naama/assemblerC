#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 100
#define MAX_LABEL_NAME_LEN 31

struct Macro {
    char name[MAX_LABEL_NAME_LEN];
    char content[MAX_LINE_LEN];
    struct Macro* next;
};

void spreadMacros(char filename[]);
