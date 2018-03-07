
/*
 Overview
 --------
 A simple RLE-like data compressor to remove sequential duplicates in data stream,
 without making any assumptions about the data distributions,
 or storing any tables too reduces overhead size.
 
     Data input: array(data, dtype=np.int8)
     Data range: 0 - 127
     Data is unstructured, and frequently contains sequential duplicated values.
 
 Algorithm description
 ---------------------
 A modified Run Length Encoding (RLE) algorithm is implemented.
 The algorithm works by iterating over the data and when a duplicate sequence is
 encountered, it sets the MSB on that value marking it as a duplicate and adds
 a byte containing the number of repetition which follows it.
 
 Example
 -------
 CASE 1: [ 1, 2, 3] -> [1, 2, 3]
 CASE 2: [ 1, 2, 2] -> [1, 130, 0]
 CASE 3: [ 1, 2, 2, 3] -> [1, 130, 0, 3]
 CASE 4: [ 1, 2, 2, 3, 3, 3] -> [1, 130, 0, 131, 1]
 
 Analysis
 --------
 Best case is when all data is duplicate values.
 Worst case is when there are no duplicates, resulting in same size output.
 If worst cases occurs often consider delta compression prior to this setup.
 
 Pros
 ----
 1) Lossless.
 2) It will never return an encoding size larger than the input.
 3) Its simple. Code & Memory. Can still be optimized to better handle duplicates.
 4) It can be implemented as an in-place operation without additional temporary buffers.
 5) It fits into a function, and does not require external functions.
 6) Can easily be used as a first pass, to feed a second phase compression method.
 6.1) Can make use of "excluded byte" values in range (128-255) as a protocol header.
 
 
 Cons
 ----
 It could regularly return the same size if no sequential duplicates are encountered,
 resulting in no compression, i.e uniform noise.
*/

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
#define BC_EXCLUDE            ((uint8_t) (128))     // Set to value in range 0-127 to exclude from RLE compression.
#define BC_MAX_DUPLICATES     ((uint8_t) (255))     // Maximum number of sequencial duplicates that can be encoded.
#define BC_CHECK_OVERFLOW                           // Can remove overflow check for release.

    if (size == 0) {
        return 0;
    }

    uint32_t i = 0;
    uint32_t j = 0;

#ifdef BC_CHECK_OVERFLOW
#define BC_IS_OVERFLOW(x, y) {if ((x) > (y)) { return BC_COMPRESSION_FAILED; }}
    
    for (i = 0; i < size; i++) {
        BC_IS_OVERFLOW(data_ptr[i], 127)
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
        v_next = (i + 1 < size) ? data_ptr[i + 1] : 128;  // Set next value if exist else set out of range for duplicate check.
        
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
            
            BC_IS_OVERFLOW(out_size + 2, size)  // Should never happen using this method.
            
            (*data_out++) = (v + 128);          // Set MSB bit of first value in sequence which is duplicate.
            (*data_out++) = cnt_duplicates - 1; // Set next byte as number of duplicates after previous.
            out_size += 2;

            i += cnt_duplicates;                // Move iteration forward to skip duplicates.
        }
        else {
            BC_IS_OVERFLOW(out_size + 1, size)  // Should never happen using this method.

            *data_out++ = v;
            out_size++;
        }
    }
    
    return out_size;
}

