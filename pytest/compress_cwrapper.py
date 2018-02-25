import numpy as np
from ctypes import cdll
from ctypes import c_uint8
from ctypes import c_int16
from ctypes import c_uint16
from ctypes import c_void_p
from ctypes import byref
from ctypes import POINTER
from ctypes import CFUNCTYPE
from numpy.ctypeslib import ndpointer as NDPOINTER

import os

COMPRESSOR_ID = "THE C IMPLEMENTATION"

LIB_NAME = 'bc_compress.dylib'
LIB_PATH = os.path.join(os.path.dirname(__file__), "../", LIB_NAME)

clib = cdll.LoadLibrary(LIB_PATH)
clib.byte_compress.argtypes = [NDPOINTER(c_uint8), c_uint16]
clib.byte_compress.restype = c_int16

DEFLATE_FUNC_CALLBACK = CFUNCTYPE(c_void_p, c_uint8)
clib.byte_decompress.argtypes = [NDPOINTER(c_uint8), c_uint16, DEFLATE_FUNC_CALLBACK]
clib.byte_decompress.restype = c_int16

# Todo Add unit test for size calc.
clib.byte_deflate_calc_size.argtypes = [POINTER(c_uint8), c_uint16]
clib.byte_deflate_calc_size.restype = c_void_p


def c_byte_encoder(data):
    if len(data) > 2**16:
        raise Warning(msg="Decoding might fail due to length overflow")

    max_len = min(len(data), 2**16)
    data = np.array(data, dtype=np.uint8)
    size = clib.byte_compress(data, c_uint16(max_len))
    return list(data[0:size])


def c_byte_decoder(data):
    if len(data) > 2**16:
        raise Warning(msg="Decoding might fail due to length overflow")

    data = np.array(data, dtype=np.uint8)
    output = []

    def deflate_handler(v):
        output.append(v)

    pyfunc_ptr = DEFLATE_FUNC_CALLBACK(deflate_handler)
    size = clib.byte_decompress(data, len(data), pyfunc_ptr)

    assert size == len(output)

    return output

