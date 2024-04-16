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
    int succesful_scan = 0, i;
    char *filename, *filenameNoExtention;

    /* verify the files we got are in the correct format and can be opened */
    verifyInput(argc, argv);

    /* run the actual scan */
    for (i = 1; i < argc; i++) {
        filename = argv[i];

        filenameNoExtention = removeExtentionFromName(filename);

        spreadMacros(filenameNoExtention);
        succesful_scan = firstScan(filenameNoExtention);


        if (succesful_scan == 0)
            secondScan(filenameNoExtention);
    }

    return succesful_scan;
}

