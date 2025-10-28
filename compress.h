#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdint.h>

int compress_data(uint8_t* data_ptr, int data_size);
void decompress_data(uint8_t* data_ptr, int data_size);

#endif // COMPRESS_H