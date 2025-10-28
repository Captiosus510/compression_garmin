#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdint.h>

long compress_data(uint8_t* data_ptr, long data_size);
uint8_t* decompress_data(uint8_t* data_ptr, long *data_size);

#endif // COMPRESS_H