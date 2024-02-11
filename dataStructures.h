#define MAX_LABEL_NAME_LEN 31

enum SymbolType {
    mdefine,
    code,
    data
};

enum SymbolUpdateMethod {
    external,
    entry,
    relocatable
};

typedef struct Symbol {
    char name[MAX_LABEL_NAME_LEN];
    enum SymbolType type;
    enum SymbolUpdateMethod method;
    int value;
    struct Symbol *next;
} Symbol;

typedef struct MemoryData {
    int decimalAddress;
    char binary[14];
    struct MemoryData *next;
} MemoryData;
