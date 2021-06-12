#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE (512)

int main(int argc, char* argv[]){
    // impose correct usage
    // TODO: read from stdin if no file is specified
    if (argc < 2){
        printf("No file specified as input\n");
        exit(0);
    }

    // buffer to read the contents from the file
    char BUFFER[BUFFER_SIZE];

    // cat the contents of each of the files on the terminal
    int file_counter = 1;
    while (argv[file_counter] != NULL){
        FILE* file_pointer = fopen(argv[file_counter], "r");

        // if unable to open file
        if (file_pointer == NULL){
            printf("a-cat: Unable to open file '%s'\n", argv[file_counter]);
            exit(1);
        }

        // read the contents into the buffer
        while (fgets(BUFFER, BUFFER_SIZE, file_pointer) != NULL){
            printf("%s", BUFFER);
        }

        fclose(file_pointer);
        ++file_counter;
    }

    return 0;
}
