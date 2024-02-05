#define MAX_LABEL_NAME_LEN 31

enum SymbolType {
    mdefine,
    code,
    data,
    external,
    entry,
    relocatable
};

typedef struct Symbol {
    char name[MAX_LABEL_NAME_LEN];
    enum SymbolType type;
    int value;
    struct Symbol *next;
} Symbol;

typedef struct MemoryData {
    int decimalAddress;
    char binary[12];
    struct MemoryData *next;
} MemoryData;

