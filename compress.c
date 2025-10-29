#include "compress.h"
#include <stdio.h>
#include <stdlib.h>

long compress_data(uint8_t* data_ptr, long data_size){
    // if the data size is less than 3, then there is no point in compressing with this algorithm (refer to README)
    if (data_size < 3) return data_size;

    // the index to write to in the buffer
    long last = 0;
    for (long i = 0; i < data_size; ){
        uint8_t val = data_ptr[i]; // current value 
        uint8_t count = 1; // number of sequential occurences of current value
        while (i + count < data_size && data_ptr[i+count] == val && count < 127){ // have to make sure that count doesn't exceed 127 because we can only store bytes in this array
            count += 1;
        }
        // if the count is 3 or more, store as count and literal in the buffer
        if (count >= 3){
            data_ptr[last] = 0x80 | count; 
            last += 1;
            data_ptr[last] = val;
            last += 1;
        }else{
            for (int j = 0; j < count; j++){
                data_ptr[last] = val;
                last += 1;
            }
        }
        i += count; // update i to go to the next value
    }

    return last;

}

uint8_t* decompress_data(uint8_t* data_ptr, long* data_size){
    // initialize the dynamic array to be 2 times the capacity of the buffer
    long capacity = 2*(*data_size);
    uint8_t* uncompressed = malloc(capacity*sizeof(uint8_t));
    if (uncompressed == NULL) exit(EXIT_FAILURE);

    // next index to write to in the buffer
    long last = 0;
    for (int i = 0; i < *data_size; ){
        // this is a literal. simply append it to uncompressed
        if(data_ptr[i] < 0x80){
            if (last >= capacity){
                capacity *= 2;
                uint8_t* temp = realloc(uncompressed, capacity*sizeof(uint8_t));
                if (temp == NULL) exit(EXIT_FAILURE);
                uncompressed = temp;
            }
            uncompressed[last] = data_ptr[i];
            last += 1;
            i += 1;
        } else{
            // append the count of the value to the uncompressed buffer
            int count = data_ptr[i] & ~(0x80);
            uint8_t val = data_ptr[i+1];
            i += 2;
            if (last + count >= capacity) {
                while (last + count >= capacity) capacity *= 2;
                uint8_t* temp = realloc(uncompressed, capacity);
                if (temp == NULL) exit(EXIT_FAILURE);
                uncompressed = temp;
            }

            for (int j = 0; j < count; j++){
                uncompressed[last] = val;
                last += 1;
            }

        }
    }

    *data_size = last;
    return uncompressed;
}

