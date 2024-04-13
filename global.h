#include <string.h>

#define MAX_LINE_LEN 100
#define MAX_LABEL_NAME_LEN 31
#define MACRO_OUTPUT_FILE_TYPE ".am"
#define ASSEMBLY_FILE_TYPE ".as"
#define COMMENT_SIGN ";"
#define LABEL_SIGN ":"


enum lineType {
    code0,
    code1,
    code2,
    dataLine,
    none
};


int startsWith(char *word, char *substring);
enum lineType findInstructionType(char line[]);
int isRegistry(char arg[]);

