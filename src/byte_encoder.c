//
//  byte_encoder.c
//  
//
//  Created by Eugene Pretorius on 2018/02/24.
//

#include "byte_encoder.h"


int16_t byte_compress( uint8_t *data_ptr, const uint16_t size)
{

#define BC_COMPRESSION_FAILED ((int16_t) ( -1))
#define BC_EXCLUDE            ((uint8_t) (127))
#define BC_MAX_DUPLICATES     ((uint8_t) (127))
#define BC_CHECK_OVERFLOW

    if (size == 0) {
        return 0;
    }

    uint32_t i = 0;
    uint32_t j = 0;

#ifdef BC_CHECK_OVERFLOW
#define BC_IS_OVERFLOW(x, y) {if ((x) > (y)) { return BC_COMPRESSION_FAILED; }}
    
    for (i = 0; i < size; i++) {
        BC_IS_OVERFLOW(data_ptr[i], 128)
    }
    
#else
#define BC_IS_OVERFLOW(x, y)
#endif
    
    uint8_t cnt_duplicates = 0;
    
    uint8_t v = data_ptr[0];
    uint8_t v_next = 128;
    uint8_t *data_out;
    
    data_out = data_ptr;
    uint32_t out_size = 0;

    for (i = 0; i < size; i++) {
        v = data_ptr[i];
        v_next = (i + 1 < size) ? data_ptr[i + 1] : 128;  // set next value if exist else set out of range for duplicate check.
        
        if ((v == v_next) && (v != BC_EXCLUDE)) {
            cnt_duplicates = 1;
            
            for (j = i + 2; j < size; j++) {
                if (v == data_ptr[j]) {
                    cnt_duplicates += 1;
                    if (cnt_duplicates >= BC_MAX_DUPLICATES) {
                        break;
                    }
                }
                else {
                    break;
                }
            }
            
            BC_IS_OVERFLOW(out_size + 2, size)
            
            (*data_out++) = (v + 128);          // Set MSB bit of first value in sequence which is duplicate.
            (*data_out++) = cnt_duplicates-1;   // Set next byte as number of duplicates after previous.
            out_size += 2;

            i += cnt_duplicates;    // move iteration forward to skip duplicates
        }
        else {
            BC_IS_OVERFLOW(out_size + 1, size)

            *data_out++ = v;
            out_size++;
        }
    }
    
    return out_size;
}

