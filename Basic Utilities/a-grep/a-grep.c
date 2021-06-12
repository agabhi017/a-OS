#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    // TODO: parse arguments to enable/disable verbosity
    // assert correct usage
    if (argc < 2){
		printf("a-grep: searchterm [files..]\n");
		exit(1);
    }

    char* line = NULL;
    size_t len = 0;
    
    // lookup in stdin
    if (argc == 2){
		while (getline(&line, &len, stdin) != -1){
			if (strstr(line, argv[1]) != NULL){
				printf("%s", line);
			}
		}
    }

    // lookup in files
    FILE* file_pointer = NULL;
    int file_counter = 2; 
    while(argv[file_counter] != NULL){
	file_pointer = fopen(argv[file_counter], "r");
	
	// if unable to open a file
		if (file_pointer == NULL){
			printf("a-grep: Cannot open file '%s'\n", argv[file_counter]);
			exit(1);
		}

		int line_counter = 1;
		while (getline(&line, &len, file_pointer) != -1){
			if (strstr(line, argv[1]) != NULL){
				printf("'%s' ==> line %d: %s", argv[file_counter], line_counter, line);
			}
			++line_counter;
		}
	
	fclose(file_pointer);
	++file_counter;
	
    }
    free(line);
   
    return 0;
}
