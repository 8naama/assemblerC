#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

#define READ_FILE_TYPE ".am"
#define COMMENT_SIGN ";"
#define LABEL_SIGN ":"

enum lineType {
    code0,
    code1,
    code2,
    dataLine,
    none
};

struct Symbol symbolTable;


int firstScan(char filename[]);
