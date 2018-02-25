//
//  byte_decoder.h
//  compress_console
//
//  Created by Eugene Pretorius on 2018/02/25.
//  Copyright © 2018 Eugene Pretorius. All rights reserved.
//

#ifndef byte_decoder_h
#define byte_decoder_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "test_debug.h"
    
    /**
     * @brief Decompresses a byte stream, allocates memory on the HEAP!
     * @param[in] data_ptr   -- Input data with range 0 - 255
     * @param[in] size       -- size of the data_ptr array.
     * @param[out] deflate   -- Output data where decompressed result will be stored.
     * @return size of decompressed, -1 if failed.
     */
    
    int16_t byte_decompress( uint8_t *data_ptr, const uint16_t size, uint8_t **deflate);
    
#ifdef __cplusplus
}
#endif

#endif /* byte_decoder_h */
