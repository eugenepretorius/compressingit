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
    
    // The decompressed data stream function type definition.
    typedef void (*deflate_callback)(uint8_t data);
    
    
    /**
     * @brief Decompresses a byte stream.
     * @param[in] data_ptr   -- Input data with range 0 - 255
     * @param[in] size       -- size of the data_ptr array.
     * @param[out] func_ptr  -- Output data sent to your callback function.
     * @return size of decompressed, -1 if failed.
     */
    
    int16_t byte_decompress( uint8_t *data_ptr, const uint16_t size, deflate_callback func_ptr);
    
    /**
     * @brief Calculate the size required to store the decompressed byte stream.
     * @param[in] data_ptr   -- Input data with range 0 - 255
     * @param[in] size       -- size of the data_ptr array.
     * @return size of decompressed, -1 if failed.
     */
    
    int16_t byte_deflate_calc_size( uint8_t *data_ptr, const uint16_t size);
    
#ifdef __cplusplus
}
#endif

#endif /* byte_decoder_h */
