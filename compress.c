#include "compress.h"

int compress_data(uint8_t* data_ptr, int data_size){
    if (data_size < 3) return data_size;

    uint8_t* curr = data_ptr;
    uint8_t* next = curr + 1;
    uint8_t count = 0x80;
    int last_written = 0;
    int flag = 0;
    for (int i = 0; i < data_size - 1; i++){
        if (*curr == *next){
            count += 1;
            if (count == 0xFF) flag = 1;
        }
        // if next element is not the same or we are at the end of the buffer (next is at index datasize-1) then raise the flag
        if (*curr != *next || i == data_size - 2){ 
            flag = 1;
        }

        if (flag){
            if (count < 0x83){
                for (int j = 0; j < count; j++){
                    data_ptr[last_written] = *curr;
                    last_written += 1;
                }
            }else{
                data_ptr[last_written] = count;
                last_written += 1;
                data_ptr[last_written] = *curr;
                last_written += 1;
            }
            count = 0x80;
            flag = 0;
        }
        
        if (i == data_size-2 && *curr != *next){
            data_ptr[last_written] = *next;
            last_written += 1;
            break;
        }
        curr += 1;
        next += 1;
    }

    return last_written;

}

// considering case where theres more than 128 bytes in a row
// considering when  all bytes in buffer are the same

void decompress_data(uint8_t* data_ptr, int data_size);

