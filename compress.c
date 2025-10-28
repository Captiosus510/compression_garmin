#include "compress.h"
#include <stdlib.h>

int compress_data(uint8_t* data_ptr, int data_size){
    // if the data size is less than 3, then there is no point in compressing with this algorithm (refer to README)
    if (data_size < 3) return data_size;

    // the index to write to in the buffer
    int last = 0;
    for (int i = 0; i < data_size; ){
        uint8_t val = data_ptr[i]; // current value 
        uint8_t count = 1; // number of sequential occurences of current value
        while (i + count < data_size && data_ptr[i+count] == val && count < 127){ // have to make sure that coutn doesn't exceed 127 because we can only store bytes in this array
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

uint8_t* decompress_data(uint8_t* data_ptr, int* data_size){
    // initialize the dynamic array to be 2 times the capacity of the buffer
    int capacity = *data_size;
    uint8_t* uncompressed = malloc(2*capacity*sizeof(uint8_t));
    if (uncompressed == NULL) exit(EXIT_FAILURE);

    // next index to write to in the buffer
    int last = 0;
    for (int i = 0; i < *data_size; ){
        // this is a literal
        if(data_ptr[i] <= 0x82){
            uncompressed[last] = data_ptr[i];
            last += 1;
            i += 1;
            while (last >= capacity){
                capacity *= 2;
                uint8_t* temp = realloc(uncompressed, capacity*sizeof(uint8_t));
                if (temp == NULL) exit(EXIT_FAILURE);
                uncompressed = temp;
            }
        } else{
            // append the count of the value to the uncompressed buffer
            int count = data_ptr[i] & ~(0x80);
            uint8_t val = data_ptr[i+1];
            i += 2;
            for (int j = 0; j < count; j++){
                uncompressed[last] = val;
                last += 1;
                while (last >= capacity){
                    capacity *= 2;
                    uint8_t* temp = realloc(uncompressed, capacity*sizeof(uint8_t));
                    if (temp == NULL) exit(EXIT_FAILURE);
                    uncompressed = temp;
                }
            }

        }
    }

    *data_size = last;
    return uncompressed;
}

