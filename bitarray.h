/* Implementation of a BitArray in C */
/*
    In C, Byte is the minimum memory that can be accessed, there is no bit access
    Byte array (char datatype) can be used to inplement the bit array
    Take a char array of size 4 ==> 4 bytes = 32 bits
    Bit array ==> || 0| 1| 2| 3| 4| 5| 6| 7|| 0| 1| 2| 3| 4| 5| 6| 7|| 0| 1| 2| 3| 4| 5| 6| 7|| 0| 1| 2| 3| 4| 5| 6| 7||
    Byte array => ||          0            ||          1            ||          2            ||          3            ||
    Indices 0 - 3 is converted to indices 0 - 31 using modular arithmetic
*/

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#define BYTE_SIZE 8

struct BitArray {
    int size;
    char *arr;
};

typedef struct BitArray ba;

void mem_alloc_bitarray(ba* bit_array, int size) {
    bit_array->size = size;
    int remainder = size % BYTE_SIZE;
    int byte_size = size / BYTE_SIZE + (remainder ? 1 : 0);

    bit_array->arr = (char*)malloc(byte_size * sizeof(char));
    memset(bit_array->arr, 0, byte_size * sizeof(char));
}

int get_bit(ba* bit_array, int index) {
    if (index < 0 || index >= bit_array->size) {
        printf("Index out of bounds\n");
        return -1;
    }
    int byte_index = index / BYTE_SIZE;
    int bit_position = index % BYTE_SIZE;
    return (bit_array->arr[byte_index] >> bit_position) & 1;
}

void set_bit(ba* bit_array, int index, int value) {
    if (index < 0 || index >= bit_array->size) {
        printf("Index out of bounds\n");
        return;
    }
    int byte_index = index / BYTE_SIZE;
    int bit_position = index % BYTE_SIZE;
    if (value) {
        bit_array->arr[byte_index] |= (1 << bit_position);
    } else {
        bit_array->arr[byte_index] &= ~(1 << bit_position);
    }
}

void free_bitarray(ba* bit_array) {
    free(bit_array->arr);
    bit_array->arr = NULL;
    bit_array->size = 0;
}

void print_bitarray(ba* bit_array) {
    for (int i = 0; i < bit_array->size; i++) {
        printf("%d", get_bit(bit_array, i));
    }
    printf("\n");
}


/* Drivers code for testing the data structure */

/* 
int main() {
    ba bit_array;
    int size = 20;

    mem_alloc_bitarray(&bit_array, size);

    set_bit(&bit_array, 3, 1);
    set_bit(&bit_array, 5, 1);
    set_bit(&bit_array, 10, 1);
    set_bit(&bit_array, 15, 1);

    print_bitarray(&bit_array);

    printf("Bit at index 5: %d\n", get_bit(&bit_array, 5));
    printf("Bit at index 7: %d\n", get_bit(&bit_array, 7));
    

    free_bitarray(&bit_array);
    return 0;
}

*/