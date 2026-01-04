/*
    Counting character frequency from a file using Map-Reduce parallalism
    The text FILE is segmented into n chunks, each chunk is passed into a Mapper which has a frequency map
    During reduction, the frequency maps of all the mappers are reduced to produce the final resultant map
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct map_args {
    int start;
    int end;
    char* buffer;
};

// arg -> address of integer pointer -> (start, end) 
void* map(void* arg) {
    struct map_args* input = (struct map_args *)arg;
    int start = input->start;
    int end = input->end;
    char* buffer = input->buffer;

    int freq[256] = {0};
    int i;

    for(i=start; i<end; i++) {
        freq[buffer[i]]++;
    }

    for(i=0; i<256; i++) {
        if (freq[i] != 0)
            printf("%c - %d\n", i, freq[i]);
    }

    return NULL;
}

int get_file_size(FILE* f) {
    int size;

    // read file and compute size
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);

    return size;
}

void file_contents_into_buffer(char* file_name, char* buffer) {
    FILE* f = fopen(file_name, "rb");

    if (f == NULL) {
        printf("Error in reading the File\n");
        return 0;
    }

    int size = get_file_size(f);

    // put the file contents into buffer
    buffer = (char *)malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
}

int main() {
    char* buffer;
    long size;
    struct map_args* chunks[4];
    int num_threads = 4;
    int chunk_size;
    
    chunk_size = size / num_threads;

    for(int i=0; i<num_threads; i++) {
        chunks[i] = (struct map_args *)malloc(sizeof(struct map_args));

    }
    
    return 0;
}