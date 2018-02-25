//
//  byte_decoder.c
//  compress_console
//
//  Created by Eugene Pretorius on 2018/02/25.
//  Copyright © 2018 Eugene Pretorius. All rights reserved.
//

#include "byte_decoder.h"

static void* safe_malloc(size_t n)
// https://stackoverflow.com/a/16298916
{
    void* p = malloc(n);
    if (!p) {
        exit(EXIT_FAILURE);
    }
    return p;
}

int16_t byte_decompress( uint8_t *data_ptr, const uint16_t size, uint8_t **deflate)
{
    uint16_t i = 0;
    uint8_t v = 0;
    uint8_t v_next = 0;
    bool v_next_skip = false;
    
    uint8_t buff[size*2];   // Allocate double input size for worst case on stack. Yeah, i know its not a good idea. ;P
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
            buff[idx++] = v - 128;
            if ((!v_next_skip) && (v_next < 128)) {
                memset(&buff[idx], (v - 128), v_next);
                idx += v_next;
            }
            else {
                buff[idx++] = v - 128;
            }
        }
        else {
            buff[idx++] = v;
        }
    }
    
    if (*deflate) {
        log_warn("Potential loss of data.");
    }
    
    *deflate = (uint8_t *) safe_malloc(idx);
    memmove(*deflate, buff, idx);

    return idx;
}


void BC_pyfreeme(char *ptr)
{
    free(ptr);
}
