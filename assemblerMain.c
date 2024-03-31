#include "dataStructures.h"
#include "firstScan.h"
#include "filesGenerator.h"


#define EXPECTED_FILE_EXTENSION ".as"


/*
This main function is temporary for test purpose.
*/
int main()
{
    int succesful_scan = 0;
    /* call macro() here to create the .am file */

    succesful_scan = firstScan("test");

    printSymbols();

    return succesful_scan;
}
