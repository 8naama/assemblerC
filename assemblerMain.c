#define EXPECTED_FILE_EXTENSION ".as"


int firstScan(char filename[]);

/*
This main function is temporary for test purpose.
*/
int main()
{
    int succesful_scan = 0;
    /* call macro() here to create the .am file */

    succesful_scan = firstScan("test");
    return succesful_scan;
}
