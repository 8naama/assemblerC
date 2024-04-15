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
    char *filename, *filenameNoExtention;

    verifyInput(argc, argv);

    /* run the actual scan */
    for (i = 1; i < argc; i++) {
        filename = argv[i];

        filenameLen = strlen(filename);
        filenameNoExtention = (char *) malloc(filenameLen+1);

        if (!filenameNoExtention) {
            printf("Error: Failed to allocate memory while generating filename.");
            exit(1);
        }

        strncpy(filenameNoExtention, filename, filenameLen - 3);
        filenameNoExtention[filenameLen - 3] = '\0';

        spreadMacros(filenameNoExtention);
        succesful_scan = firstScan(filenameNoExtention);


        if (succesful_scan == 0)
            secondScan(filenameNoExtention);
    }

    return succesful_scan;
}

