#if (BE_COMPRESSION_TESTING == 1)

#include <stdio.h>
#include "byte_encoder.h"

void test_example(void)
{
    uint8_t data_ptr[] = {
        0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
        0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 };
    
    uint16_t size = byte_compress(data_ptr, sizeof(data_ptr));
    
    printf("byte_compress %d", size);
}


int main(void)
{
    test_example();
    return 0;
}

#endif  // BE_COMPRESSION_TESTING
