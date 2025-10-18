/* Implementation of bloom filter using custom hash and bitarray */

#include "hash.h"
#include "bitarray.h"

struct BloomFilter {
    int size;
    int num_hashes;
    ba bit_array;
};

void init_bloom_filter(struct BloomFilter* bf, int size, int num_hashes) {
    bf->size = size;
    bf->num_hashes = num_hashes;
    mem_alloc_bitarray(&bf->bit_array, size);
}

void add_element(struct BloomFilter* bf, const char* element) {
    for (int i = 0; i < bf->num_hashes; i++) {
        unsigned long hash_value = simple_hash(element, i);
        int index = hash_value % bf->size;
        set_bit(&bf->bit_array, index, 1);
    }
}

int check_element(struct BloomFilter* bf, const char* element) {
    for (int i = 0; i < bf->num_hashes; i++) {
        unsigned long hash_value = simple_hash(element, i);
        int index = hash_value % bf->size;
        if (get_bit(&bf->bit_array, index) == 0) {
            return 0; // defnitely not in the set
        }
    }
    return 1; // possibly in the set
}

void free_bloom_filter(struct BloomFilter* bf) {
    free_bitarray(&bf->bit_array);
    bf->size = 0;
    bf->num_hashes = 0;
}


int main() {
    struct BloomFilter bf;
    init_bloom_filter(&bf, 100, 5);
    add_element(&bf, "hello");
    add_element(&bf, "world");

    printf("Check 'hello': %d\n", check_element(&bf, "hello")); // Expected: 1
    printf("Check 'world': %d\n", check_element(&bf, "world"));
    printf("Check 'bloom': %d\n", check_element(&bf, "bloom")); // Expected: 0
    
    free_bloom_filter(&bf);
    return 0;
}