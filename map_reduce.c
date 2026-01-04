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
    long chunk_size = size / N;
    long residue = size % N;
    long start = 0;
    long end = 0;

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

int* reduce(struct map_args** chunks, int N) {
    int* total_freq;

    total_freq = (int *)calloc(256, sizeof(int));

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < 256; j++) {
            total_freq[j] += chunks[i]->local_freq[j];
        }
    }

    return total_freq;
}

// map_reduce_char_frequency(file_name, number of mappers / threads) -> returns int* as a result
int* map_reduce_char_frequency(char* file_name, int num_threads) {
    long size;
    struct map_args** chunks;
    int N = num_threads;
    int* result;
    pthread_t threads[N];

    char* buffer = file_contents_into_buffer(file_name, &size);

    chunks = (struct map_args **)malloc(N * sizeof(struct map_args *));
    for (int i=0; i<N; i++) {
        chunks[i] = (struct map_args *)malloc(sizeof(struct map_args));
    }
    
    divide_by_chunks(chunks, buffer, size, N);

    /* mapping phase is parallelized*/
    for(int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, map, (void*)chunks[i]);
    }
    
    for(int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    result = reduce(chunks, N);
    
    for(int i = 0; i < N; i++) {
        free(chunks[i]);
    }
    free(chunks);
    free(buffer);

    return result;
}

void bench_marking(char* file_name) {
    struct timespec start, end;
    double elapsed_time;
    int* result;

    for(int i = 1; i < 9; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        result = map_reduce_char_frequency(file_name, i);
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("For %d threads: It took %lf seconds\n", i, elapsed_time);
        free(result);
    }
}


int main() {    
    /*
    int* result = map_reduce_char_frequency("test.txt", 4);
    for(int i = 0; i < 256; i++) {
        if (result[i] != 0) {
            printf("%c - %d\n", (char)i, result[i]);
        }
    }
    free(result);
    */
    bench_marking("test.txt");

    return 0;
}