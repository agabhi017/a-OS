#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SIZE (2147483)

int main(){

    FILE* fp = fopen("test-gen.1", "w");
    if (fp == NULL){
        exit(1);
    }

    char buffer[SIZE];
    for (int j = 0; j < 2001; j++){
        for (int i = 0; i < SIZE; i++){
            buffer[i] = 'a';
        }
        fwrite(buffer, sizeof(char), SIZE, fp);
    }
    fclose(fp);
    
    return 0;
}