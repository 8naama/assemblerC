#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define OP_CODE_START_INDEX 4
#define FIRST_WORD_SRC_START_INDEX 8
#define FIRST_WORD_DST_START_INDEX 10
#define ARG_TYPE_START_INDEX 12
#define NEXT_WORD_REG_SRC_INDEX 6
#define NEXT_WORD_REG_DEST_INDEX 9
#define IMMIDIATE_SIGN "#"
#define IMMIDIATE_BINARY "00"
#define DIRECT_BINARY "01"
#define FIXED_INDEX_BINARY "10"
#define DIRECT_REGISTER_BINARY "11"


enum argType {
    immediate,
    direct,
    fixedIndex,
    directRegister,
    noArg
};


void secondScan(char filename[]);


