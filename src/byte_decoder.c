//
//  byte_decoder.c
//  compress_console
//
//  Created by Eugene Pretorius on 2018/02/25.
//  Copyright © 2018 Eugene Pretorius. All rights reserved.
//

#include "byte_decoder.h"

int16_t byte_decompress( uint8_t *data_ptr, const uint16_t size, deflate_callback func_ptr)
{
    #define YIELD(v) if (func_ptr) { func_ptr((v)); }
    
    uint16_t i = 0;
    uint16_t j = 0;
    uint8_t v = 0;
    uint8_t v_next = 0;
    bool v_next_skip = false;
    uint16_t idx = 0;
    
    for (i = 0; i < size; i++) {
        v = data_ptr[i];
        
        if (i + 1 < size) {
            v_next = data_ptr[i + 1];
        }
        else {
            v_next_skip = true;
        }
        
        if (v >= 128) {
            YIELD(v - 128)
            idx++;
            if ((!v_next_skip) && (v_next < 128)) {
                
                for (j = 0; j < v_next + 1; j++) {
                    YIELD(v - 128)
                }
                idx += v_next + 1;
                i += 1;     // skip next since it is a length value.
            }
            else {
                YIELD(v - 128)
                idx++;
            }
        }
        else {
            YIELD(v)
            idx++;
        }
    }
    
    return idx;
}


int16_t byte_deflate_calc_size( uint8_t *data_ptr, const uint16_t size)
{
    return byte_decompress(data_ptr, size, NULL);
}

