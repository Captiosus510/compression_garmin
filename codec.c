#include "compress.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    if (argc < 2 || argv[1][0] != 'c' || argv[1][0] == 'd') {
        fprintf(stderr, "Usage: %s [c|d]\n", argv[0]);
        return 1;
    }
    
    // read stdin
    fseek(stdin, 0, SEEK_END);
    long size = ftell(stdin);
    rewind(stdin);

    uint8_t* data = malloc(size);
    fread(data, 1, size, stdin);

    if (argv[1][0] == 'c'){
        long new_size = compress_data(data, size);
        fwrite(data, 1, new_size, stdout);
    }else if (argv[1][0] == 'd'){
        long s = size;
        uint8_t* dec = decompress_data(data, &s);
        fwrite(dec, 1, s, stdout);
        free(dec);
    }

    free(data);
    return 0;
}