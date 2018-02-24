//
//  byte_encoder.h
//  
//
//  Created by Eugene Pretorius on 2018/02/24.
//

#ifndef byte_encoder_h
#define byte_encoder_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdint.h>
#include <stdbool.h>


#define BE_COMPRESSION_FAILED ((uint16_t) (-1))
    
    
/**
 * @brief Compresses a byte stream inplace.
 * @param[in] data_ptr   -- Input data with range 0 - 127, values are altered.
 * @param[in] size       -- size of the data_ptr array.
 * @return size of compression, -1 if failed.
 */

uint16_t byte_compress( uint8_t *data_ptr, const uint16_t size);

#ifdef __cplusplus
}
#endif
    
#endif // byte_encoder_h
