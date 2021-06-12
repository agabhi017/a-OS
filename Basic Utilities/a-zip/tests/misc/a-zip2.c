#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define READ_BUFFER_SIZE (65536)
#define WRITE_BUFFER_SIZE (50)

typedef struct 
{
    uint32_t count;
    uint8_t character;
}__attribute__((__packed__))
m_data;


int main(int argc, char* argv[]){

    // impose correct usage
    if (argc != 2){
        printf("a-zip: No file specified. Correct usage ==> ./a-zip filename\n");
        exit(0);
    }

    // opening the given file
    FILE* file_pointer = NULL;
    file_pointer = fopen(argv[1], "r");
    if (file_pointer == NULL){
        printf("a-zip: Unable to open file '%s'\n", argv[1]);
        exit(1);
    }

    // reading the file into the buffer
    char READ_BUFFER[READ_BUFFER_SIZE];
    //m_data WRITE_BUFFER[WRITE_BUFFER_SIZE];

    //int write_buffer_index = 0;

    while (fread(READ_BUFFER, sizeof(char), READ_BUFFER_SIZE, file_pointer) != 0){
        int len = strlen(READ_BUFFER);
        
        for (int i = 0; i < len; i++){
            int temp_count = 1;
            char temp_char = READ_BUFFER[i];
            while(i < len - 1 && READ_BUFFER[i] == READ_BUFFER[i + 1]){
                ++temp_count;
                ++i;
            }

            // storing the character and the count in the temp write buffer
            //WRITE_BUFFER[write_buffer_index].count = temp_count;
            //WRITE_BUFFER[write_buffer_index].character = temp_char;
            //++write_buffer_index;

            // if the write buffer is full, fwrite the buffer 
            /*
            if (write_buffer_index >= WRITE_BUFFER_SIZE){
                write_buffer_index = 0;
                fwrite(WRITE_BUFFER, sizeof(m_data), WRITE_BUFFER_SIZE, stdout);
            }
            */

           fwrite(&temp_count, sizeof(int), 1, stdout);
           fwrite(&temp_char, sizeof(char), 1, stdout);
        }
    }

    // fwrite whatever is remaining in the temp write buffer
    /*
    if (write_buffer_index > 0){
        fwrite(WRITE_BUFFER, sizeof(m_data), write_buffer_index, stdout);
    }
    */
    fclose(file_pointer);

    return 0;
}