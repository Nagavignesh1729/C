/*
    Counting character frequency from a file using Map-Reduce parallalism
    The text FILE is segmented into n chunks, each chunk is passed into a Mapper which has a frequency map
    During reduction, the frequency maps of all the mappers are reduced to produce the final resultant map
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* map(void* arg) {
    printf("This is a function\n");
    return NULL;
}

void* print_thread(void* arg) {
    int freq[256] = {0};
    return NULL;
}

int main() {
    FILE* f;
    char* buffer;
    long size;

    f = fopen("test.txt", "rb");
    
    if (f == NULL) {
        printf("Error in reading the File\n");
        return 0;
    }

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);

    buffer = (char *)malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = "\0";

    fclose(f);

    return 0;
}