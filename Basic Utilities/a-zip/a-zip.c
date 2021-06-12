#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>     // for uint32_t and uint8_t

#define READ_BUFFER_SIZE (65536)        // 64KB buffer
#define WRITE_BUFFER_SIZE (204800)      // 1MB buffer

typedef struct 
{
    uint32_t count;
    uint8_t character;
}__attribute__((__packed__))
m_data;

static char READ_BUFFER[READ_BUFFER_SIZE];
static m_data WRITE_BUFFER[WRITE_BUFFER_SIZE];

static inline void write_buffer_to_file(int size){
    if (size > 0){
        fwrite(WRITE_BUFFER, sizeof(m_data), size, stdout);
    }
}

static inline void write_to_buffer(char character, int char_count, int* buffer_index){
    if (buffer_index != NULL && *buffer_index >= 0 && *buffer_index < WRITE_BUFFER_SIZE && char_count > 0){
        WRITE_BUFFER[*buffer_index].count = char_count;
        WRITE_BUFFER[*buffer_index].character = character;
        *buffer_index += 1;
    }
    else if (buffer_index != NULL && *buffer_index >= WRITE_BUFFER_SIZE){
        write_buffer_to_file(WRITE_BUFFER_SIZE);
        *buffer_index = 0;
        write_to_buffer(character, char_count, buffer_index);
    }
}

int main(int argc, char* argv[]){

    // impose correct usage
    if (argc < 2){
        printf("a-zip: No file specified. Correct usage ==> ./a-zip file [file ...]\n");
        exit(0);
    }

    char last_char = 0;
    int last_char_count = 0;

    int buffer_len = 0;
    int write_buffer_index = 0;

    int file_index = 1;
    FILE* file_pointer = NULL;

    // iterating over files
    while (argv[file_index] != NULL){
        
        file_pointer = fopen(argv[file_index], "r");
        if (file_pointer == NULL){
            printf("a-zip: Unable to open file '%s'\n", argv[file_index]);
            exit(1);
        }

        // iterating over a file
        while ((buffer_len = fread(READ_BUFFER, sizeof(char), READ_BUFFER_SIZE, file_pointer)) != 0){

            // maintain last char count if the first character matches
            if (last_char == READ_BUFFER[0]){
                if (write_buffer_index > 0){
                    --write_buffer_index;               // update buffer index if its non-zero
                }
            }
            else {
                last_char_count = 0;
            }

            int curr_char_count = 0;
            char curr_char = 0;

            // iterating over a file's buffer
            for (int i = 0; i < buffer_len; i++){
                curr_char_count = last_char_count + 1;
                curr_char = READ_BUFFER[i];
                last_char_count = 0;                // reset the counter

                // match all chars that match the temp char
                while(i < buffer_len - 1 && READ_BUFFER[i] == READ_BUFFER[i + 1]){
                    ++curr_char_count;
                    ++i;

                    // check if the char count is reaching INT_MAX; if yes then write to buffer
                    if (curr_char_count == INT32_MAX){
                        write_to_buffer(curr_char, curr_char_count, &write_buffer_index);
                        curr_char_count = 0;
                    }
                }

                // write character and its count to the write buffer
                write_to_buffer(curr_char, curr_char_count, &write_buffer_index);
            }

            // store the char and count in last char's vars
            last_char = curr_char;
            last_char_count = curr_char_count;
        }

        fclose(file_pointer);
        ++file_index;
    }

    // fwrite whatever is remaining in the temp write buffer
    write_buffer_to_file(write_buffer_index);

    return 0;
}