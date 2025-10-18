/* Implementation of hash function in C */
/*
    For now, takes a simple string and returns a hash value using a basic algorithm.
*/

unsigned long simple_hash(const char *str, int seed=5381) {
    unsigned long hash = seed;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash%100;
}

/* 
int main() {
    const char *test_str = "Hello, World!";
    unsigned long hw = simple_hash(test_str), a = simple_hash("a"), aa = simple_hash("aa"), ab = simple_hash("ab");

    printf("Hash value for '%s': %lu\n", test_str, hw);
    printf("Hash value for 'a': %lu\n", a);
    printf("Hash value for 'aa': %lu\n", aa);
    printf("Hash value for 'ab': %lu\n", ab);
    return 0;
}
*/