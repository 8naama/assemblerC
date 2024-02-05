#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"


#define ENTRY_FILE_TYPE ".en"
#define EXTERNAL_FILE_TYPE ".ex"
#define OBJECT_FILE_TYPE ".ob"

FILE *curr_file;


void write_to_file(char filename[], char newline[]) 
{
    curr_file = fopen(filename, "a");
    fprintf(curr_file, "%s\n", newline);
    fclose(curr_file);
}


void _generate_entry_and_external_files(char filename[], struct Symbol *symbols_head)
{
    /* Initializing file names and the current Symbol */
    char entry_file_name[100];
    char external_file_name[100];
    Symbol *curr_symbol = symbols_head;
    Symbol *tmp;
    sprintf(entry_file_name, "%s%s", filename, ENTRY_FILE_TYPE);
    sprintf(external_file_name, "%s%s", filename, EXTERNAL_FILE_TYPE);

    /* Going over all the Symbols */
    while (curr_symbol) {
        enum SymbolType curr_type = curr_symbol->type;
        
        char newline[MAX_LABEL_NAME_LEN+6];
        sprintf(newline, "%s\t%d", curr_symbol->name, curr_symbol->value);
        
        /* If external or entry, then save to the relevant file */
        if (curr_type == entry)
            write_to_file(entry_file_name, newline);
        else if (curr_type == external)
            write_to_file(external_file_name, newline);

        tmp = curr_symbol->next;
        /*free(curr_symbol);*/ /* TODO: check how memory cleaning works*/
        curr_symbol = tmp;
    }
}


char *_encode_word(char word)
{
    printf("skip encode word\n");
    return "*&!##!*";
}


void _generate_object_file(char filename[], int inst_count, int data_count, struct MemoryData *binary_word_head) 
{
    /* Initializing file names and the current Symbol */
    char object_file_name[100];
    char header[5];
    MemoryData *curr_line = binary_word_head;
    MemoryData *tmp;
    sprintf(object_file_name, "%s%s", filename, OBJECT_FILE_TYPE);
    sprintf(header, "\t%d %d", inst_count, data_count);
    
    /* Writing Header to the Object file */
    write_to_file(object_file_name, header);

    /* Going over all the memory lines */    
    while (curr_line) {
        char newline[50];

        sprintf(newline, "%d\t%s", curr_line->decimalAddress, _encode_word(curr_line->binary));
        write_to_file(object_file_name, newline);

        tmp = curr_line->next;
        /*free(curr_line);*/ /* TODO: check how memory cleaning works*/
        curr_line = tmp;
    }
}


/*
Generates files named after the given 'filename', with file types matching ENTRY_FILE_TYPE, EXTERNAL_FILE_TYPE and OBJECT_FILE_TYPE.
The files content depends on the given 'symbols_head' and 'binary_word_head' structures content.

Input: string filename, Symbol structure, instructions lines count, data lines count and MemoryData structure.
Output: None, but generates between 1 to 3 files, depending on the given structures content.
*/
void generate_files(char filename[], struct Symbol *symbols_head, int inst_count, int data_count, struct MemoryData *binary_word_head)
{
    _generate_entry_and_external_files(filename, symbols_head);
    _generate_object_file(filename, inst_count, data_count, binary_word_head);
}
