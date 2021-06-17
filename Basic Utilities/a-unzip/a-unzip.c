/*
a-unzip: A unix utility that unzips files zippped using a-zip
which uses a simple RLE method to zip files. 
It processes 5 bytes at a time of which the first 4 bytes correspond to the frequency of the 
character stored as the 5th byte.
*/

#include <stdint.h>  // for uint32_t and uint8_t
#include <stdio.h>
#include <stdlib.h>

#define READ_BUFFER_SIZE (12288)        // 60 KB buffer     
//#define WRITE_BUFFER_SIZE (102400)      // 100 KB buffer


// a-zip uses a buffer of m_data struct to write to files 
typedef struct 
{
    uint32_t count;
    uint8_t character;
}__attribute__((__packed__))
m_data;

static m_data READ_BUFFER[READ_BUFFER_SIZE];
//static char WRITE_BUFFER[WRITE_BUFFER_SIZE];

int main(int argc, char* argv[]){

    if (argc != 2){                 // impose correct usage
        printf("a-unzip: No file specified. Correct usage ==> a-unzip file");
        exit(0);
    }

    FILE* file_pointer = fopen(argv[1], "r");
    if (file_pointer == NULL){
        printf("a-unzip: Unable to open file '%s'", argv[1]);
        exit(1);
    }

    //int write_buffer_index = 0;
    int buffer_len = 0;

    while ((buffer_len = fread(READ_BUFFER, sizeof(m_data), READ_BUFFER_SIZE, file_pointer)) != 0){
        char character = 0;
        int character_count = 0;

        for (int i = 0; i < buffer_len; i++){
            character = READ_BUFFER[i].character;
            character_count = (int)READ_BUFFER[i].count;
            for (int j = 0; j < character_count; j++){
                printf("%c", character);
            }

        }
    }
    fclose(file_pointer);
    return 0;
}