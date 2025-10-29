#include "compress.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t* read_stdin(long* out_size) {
    // read from stdin until end of input
    long capacity = 1024;
    long size = 0;
    uint8_t* buffer = malloc(capacity);
    if (!buffer) exit(EXIT_FAILURE);

    // keep adding character to buffer until EOF. double capacity if capacity reached
    int c;
    while ((c = fgetc(stdin)) != EOF) {
        buffer[size++] = (uint8_t)c;
        if (size >= capacity) {
            capacity *= 2;
            uint8_t* temp = realloc(buffer, capacity);
            if (!temp) exit(EXIT_FAILURE);
            buffer = temp;
        }
    }

    *out_size = size;
    return buffer;
}

int main(int argc, char** argv){
    setvbuf(stdout, NULL, _IONBF, 0);  // disable buffering so stdout is always flushed
    if (argc < 2 || (argv[1][0] != 'c' && argv[1][0] != 'd')) {
        fprintf(stderr, "Usage: %s [c|d]\n", argv[0]);
        return 1;
    }
    // get data from stdin
    long size;
    uint8_t* data = read_stdin(&size);
    //compress or decompress based on argv and then write to stdout
    if (argv[1][0] == 'c'){
        long new_size = compress_data(data, size);
        fwrite(data, 1, new_size, stdout);
    }else if (argv[1][0] == 'd'){
        long s = size;
        uint8_t* dec = decompress_data(data, &s);
        fwrite(dec, 1, s, stdout);
        free(dec);
    }

    // free memory and return
    free(data);
    return 0;
}