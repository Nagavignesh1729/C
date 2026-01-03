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

int main() {
    FILE* f;
    char* buffer;
    long size;
    struct map_args* chunks[4];
    pthread_t t1, t2;

    for(int i=0; i<2; i++) {
        chunks[i] = (struct map_args *)malloc(sizeof(struct map_args));
    }
    chunks[0]->start = 0;
    chunks[0]->end = 30;

    chunks[1]->start = 30;
    chunks[1]->end = 80;
    

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
    buffer[size] = '\0';

    chunks[0]->buffer = buffer;
    chunks[1]->buffer = buffer;

    printf("Size of test file: %ld\n", size);

    pthread_create(&t1, NULL, map, (void *)chunks[0]);
    pthread_create(&t2, NULL, map, (void *)chunks[1]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    fclose(f);

    return 0;
}