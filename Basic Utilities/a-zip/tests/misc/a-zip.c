#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define READ_BUFFER_SIZE (65536)        // 64KB buffer
#define WRITE_BUFFER_SIZE (204800)      // 1MB sized buffer

typedef struct 
{
    uint32_t count;
    uint8_t character;
}__attribute__((__packed__))
m_data;

static char READ_BUFFER[READ_BUFFER_SIZE];
static m_data WRITE_BUFFER[WRITE_BUFFER_SIZE];

/*
void write_to_buffer(char character, int count, int* buffer_index){
    if (*buffer_index > 0 && *buffer_index < WRITE_BUFFER_SIZE){
        WRITE_BUFFER[write_buffer_index].count = curr_char_count;
        WRITE_BUFFER[write_buffer_index].character = curr_char;
    }
}

void write_buffer(int size, FILE* stream){

}
*/
// TODO: 
// 1. handle cases when the frequency is larger than INT_MAX
// 2. handle edge cases where the write buffer might become full exactly at the last iteration of the freq count for loop
// 3. handle multiple files

int main(int argc, char* argv[]){

    // impose correct usage
    if (argc != 2){
        printf("a-zip: No file specified. Correct usage ==> ./a-zip filename\n");
        exit(0);
    }

    // opening the given file
    FILE* file_pointer = fopen(argv[1], "r");
    if (file_pointer == NULL){
        printf("a-zip: Unable to open file '%s'\n", argv[1]);
        exit(1);
    }

    int write_buffer_index = 0;

    // check if the last character of a read buffer matches the first character of the next read buffer
    char last_char = 0;
    int last_char_count = 0;

    int buffer_len = 0;
    while ((buffer_len = fread(READ_BUFFER, sizeof(char), READ_BUFFER_SIZE, file_pointer)) != 0){
        for (int i = 0; i < buffer_len; i++){
            int curr_char_count = 1;
            char curr_char = READ_BUFFER[i];
            
            // check if the first char of the current buffer matches the last char of previous buffer and update counts
            if (i == 0 && last_char_count > 0 && last_char == curr_char){
                curr_char_count += last_char_count;
                if (write_buffer_index > 0){
                    --write_buffer_index;               // update buffer index if its non-zero
                }
            }

            // match all chars that match the temp char
            while(i < buffer_len - 1 && READ_BUFFER[i] == READ_BUFFER[i + 1]){
                ++curr_char_count;
                ++i;

                // check if the char count is reaching INT_MAX; if yes then write to buffer
                if (curr_char_count == INT32_MAX){
                    curr_char_count = 0;

                    WRITE_BUFFER[write_buffer_index].count = curr_char_count;
                    WRITE_BUFFER[write_buffer_index].character = curr_char;
                    ++write_buffer_index;
                }
            }

            // if end of the buffer is reached store the char and count in last char's vars
            if (i == buffer_len - 1){
                last_char = curr_char;
                last_char_count = curr_char_count;
            }

            // storing the character and the count in the write buffer
            if (curr_char_count > 0){
                WRITE_BUFFER[write_buffer_index].count = curr_char_count;
                WRITE_BUFFER[write_buffer_index].character = curr_char;
                ++write_buffer_index;
            }

            // if the write buffer is full, fwrite the buffer 
            if (write_buffer_index >= WRITE_BUFFER_SIZE){
                write_buffer_index = 0;
                fwrite(WRITE_BUFFER, sizeof(m_data), WRITE_BUFFER_SIZE, stdout);
            }
        }
    }

    // fwrite whatever is remaining in the temp write buffer
    if (write_buffer_index > 0){
        fwrite(WRITE_BUFFER, sizeof(m_data), write_buffer_index, stdout);
    }

    //free(WRITE_BUFFER);
    fclose(file_pointer);

    return 0;
}