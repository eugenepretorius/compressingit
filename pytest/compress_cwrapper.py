import numpy as np
from ctypes import cdll
from ctypes import c_uint8
from ctypes import c_int16
from ctypes import c_uint16
from numpy.ctypeslib import ndpointer as NDPOINTER

import os

COMPRESSOR_ID = "THE C IMPLEMENTATION"

LIB_NAME = 'bc_compress.dylib'
LIB_PATH = os.path.join(os.path.dirname(__file__), "../", LIB_NAME)

clib = cdll.LoadLibrary(LIB_PATH)
clib.byte_compress.argtypes = [NDPOINTER(c_uint8), c_uint16]
clib.byte_compress.restype = c_int16


def c_byte_encoder(data):
    if len(data) > 2**16:
        raise Warning(msg="Decoding might fail due to length overflow")

    max_len = min(len(data), 2**16)
    data = np.array(data, dtype=np.uint8)
    size = clib.byte_compress(data, c_uint16(max_len))
    return list(data[0:size])
