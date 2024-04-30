#include "dataStructures.h"
#include "firstScan.h"
#include "macro.h"
#include "global.h"
#include "filesGenerator.h"
#include "secondScan.h"
#include <string.h>


/*
The main function of the program.
Reads assembly files, and generates file with macro spread out, object file based on the assembly content, and entry or external files if the code had such variables in it.

Input: file names with ASSEMBLY_FILE_TYPE extention
Output: 0 if successful, 1 otherwise. 
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

