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
    int local_freq[256];
};

// arg -> address of integer pointer -> (start, end) 
void* map(void* arg) {
    struct map_args* input = (struct map_args *)arg;
    int start = input->start;
    int end = input->end;
    char* buffer = input->buffer;
    int* freq = input->local_freq;
    int i;

    for(i=0; i<256; i++) {
        freq[i] = 0;
    }

    for(i=start; i<end; i++) {
        freq[(unsigned char)buffer[i]]++;
    }

    return NULL;
}

long get_file_size(FILE* f) {
    long size;

    // read file and compute size
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);

    return size;
}

char* file_contents_into_buffer(char* file_name, long* size) {
    FILE* f = fopen(file_name, "rb");
    char* buffer;

    if (f == NULL) {
        printf("Error in reading the File\n");
        return NULL;
    }

    *size = get_file_size(f);

    // put the file contents into buffer
    buffer = (char *)malloc(*size + 1);
    fread(buffer, 1, *size, f);
    buffer[*size] = '\0';

    fclose(f);

    return buffer;
}

void divide_by_chunks(struct map_args** chunks, char* buffer, long size, int N) {
    int chunk_size = size / N;
    int residue = size % N;
    int start = 0;
    int end = 0;

    for(int i=0; i<N; i++) {
        if (i < residue) {
            end = end + chunk_size + 1;
        } else {
            end = end + chunk_size;
        }

        chunks[i]->start = start;
        chunks[i]->end = end;
        chunks[i]->buffer = buffer;
        
        //printf("%d - %d, %d\n", start, end, end-start);

        start = end;
    }
}

int main() {
    long size;
    struct map_args** chunks;
    int N = 4;

    char* buffer = file_contents_into_buffer("test.txt", &size);
    
    chunks = (struct map_args **)malloc(sizeof(struct map_args *));
    for (int i=0; i<N; i++) {
        chunks[i] = (struct map_args *)malloc(sizeof(struct map_args));
    }

    printf("%s\n%ld\n", buffer, size);
    divide_by_chunks(chunks, buffer, size, N);

    free(buffer);

    return 0;
}