#if (BE_COMPRESSION_TESTING == 1)

#include <stdio.h>
#include "byte_encoder.h"
#include "test_debug.h"

#define BC_TEST_PASS    (0)
#define BC_TEST_FAIL    (1)

int test_common_cmp(
        char const *caller_name,
        uint8_t *test_data_ptr,
        uint8_t *expected_data_ptr,
        const uint16_t size,
        const uint16_t exp_size)
{
    debug("\n");
    debug("%s", caller_name);
    debug(" ... ");
    
    if (exp_size != size) {
        log_err("Size does not match expected length.");
        debug("FAIL");
        return BC_TEST_FAIL;
    }
    
    uint32_t i = 0;
    
//    debug("\n[");
    for (i=0; i < size; i++) {
        if (test_data_ptr[i] != expected_data_ptr[i]) {
            debug(" FAIL");
            log_err("Value mismatch at position.");
            return BC_TEST_FAIL;
        }
//        debug("%d, ", test_data_ptr[i]);
    }
//    debug("]\n");
    
    debug(" ok");
    return BC_TEST_PASS;
}

int test_single_value(void)
{
    uint8_t tst[] = { 65 };
    uint8_t exp[] = { 65 };
    
    uint16_t size = byte_compress(tst, sizeof(tst));

    return test_common_cmp(__func__, tst, exp, size, sizeof(exp));
}

int test_case1(void)
{
    uint8_t tst[] = { 1, 2, 3 };
    uint8_t exp[] = { 1, 2, 3 };
    
    uint16_t size = byte_compress(tst, sizeof(tst));
    
    return test_common_cmp(__func__, tst, exp, size, sizeof(exp));
}

int test_monotonic_dec(void)
{
    uint8_t tst[] = { 9,  8,  8,  6,  5,  5,  3,  2,  1,  0};
    uint8_t exp[] = { 9, 136, 0, 6, 133, 0, 3, 2, 1, 0 };
    
    uint16_t size = byte_compress(tst, sizeof(tst));
    
    return test_common_cmp(__func__, tst, exp, size, sizeof(exp));

}

int test_example(void)
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

    return test_common_cmp(__func__, data_ptr, expected, size, sizeof(expected));
}


int main(void)
{
    int res = BC_TEST_PASS;
    res += test_example();
    res += test_case1();
    res += test_single_value();
    res += test_monotonic_dec();
    
    debug("\n\n-------------\n");

    if (res == 0) {
        debug("Tests PASSED\n");
    }
    else {
        debug("Tests FAILED (%d) \n", res);
    }
    
    return res;
}

#endif  // BE_COMPRESSION_TESTING
