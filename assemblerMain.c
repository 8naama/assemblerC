#include "dataStructures.h"
#include "firstScan.h"
#include "macro.h"
#include "global.h"
#include "filesGenerator.h"
#include "secondScan.h"
#include <string.h>


/*
This main function is temporary for test purpose.
*/
int main(int argc, char *argv[])
{
    int succesful_scan = 0, i, filenameLen;
    char *filename, *fileExtension, *filenameNoExtention;
    FILE * fd;


    if (argc == 1) {
        printf("Error: Please enter at least one %s file as argument\n", ASSEMBLY_FILE_TYPE);
        exit(1);
    }

    /* verify the filename length, extension type and permissions */
    for (i = 1; i < argc; i++) {
        filename = argv[i];

        if (strlen(filename) < 4) {
            printf("Error: provided filename %s is too short.\n", filename);
            exit(1);
        }
        else {
            fileExtension = &filename[strlen(filename)-3];

            if (strcmp(fileExtension, ASSEMBLY_FILE_TYPE) != 0) {
                printf("Error: file %s has extension %s but expected %s\n", filename, fileExtension, ASSEMBLY_FILE_TYPE);
                exit(1);
            }
        }
        if (!(fd = fopen(filename, "r"))) {
            printf("Error: File %s does not exist or missing read permissions\n", filename);
            exit(1);
        }
    }

    for (i = 1; i < argc; i++) {
        filename = argv[i];

        filenameLen = strlen(filename);
        filenameNoExtention = (char *) malloc(filenameLen+1);
        strncpy(filenameNoExtention, filename, filenameLen - 3);
        filenameNoExtention[filenameLen - 3] = '\0';


        spreadMacros(filenameNoExtention);
        succesful_scan = firstScan(filenameNoExtention);

        printSymbols();

        if (succesful_scan == 0) {
            printf("call second scan here\n");
            myfunc("MAIN");
            myfunc("other");
         }
        /*generateObjectFile("object", 100, "00000000111000");*/
    }

    return succesful_scan;
}
