#if (BE_COMPRESSION_TESTING == 1)

#include <stdio.h>
#include "byte_encoder.h"

void test_example(void)
{
    uint8_t data_ptr[] = {
        0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
        0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09
    };
    
    uint8_t expected[] = {
        3, 116, 132, 1, 181, 0, 228, 2,
        128, 3, 86, 69, 214, 1, 137, 1
    };
    
    uint16_t size = byte_compress(data_ptr, sizeof(data_ptr));

    printf("byte_compress %d", size);
    uint32_t i = 0;
    
    printf("\n[");
    for (i=0; i<size; i++) {
        if (data_ptr[i] != expected[i]) {
            printf("\nERROR %d \n", i);
        }
        printf("%d, ", data_ptr[i]);
    }
    printf("]\n");
        
}


int main(void)
{
    test_example();
    return 0;
}

#endif  // BE_COMPRESSION_TESTING
