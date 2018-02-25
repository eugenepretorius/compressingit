import os
from random import randint
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import unittest

# TEST compress_duplicator.py
# from compress_duplicator import encoder_duplicates as encoder
# from compress_duplicator import decoder_duplicates as decoder
# from compress_duplicator import COMPRESSOR_ID

# TEST compress_duplicator_nibble.py
# from compress_duplicator_nibble import encoder_duplicates_with_nibble as encoder
# from compress_duplicator_nibble import decoder_duplicates_with_nibble as decoder
# from compress_duplicator_nibble import COMPRESSOR_ID

# TEST c-code implementation
from compress_cwrapper import c_byte_encoder as encoder
from compress_cwrapper import c_byte_decoder as decoder
from compress_duplicator import encoder_duplicates
from compress_duplicator import decoder_duplicates
from compress_cwrapper import COMPRESSOR_ID


def _common_enc_dec(test_data):
    encoded_bytes = encoder(test_data)
    decoded_bytes = decoder(encoded_bytes)

    if COMPRESSOR_ID == "THE NIBBLER" and len(test_data) > 4:
        assert len(encoded_bytes) <= len(test_data)

    assert list(decoded_bytes) == list(test_data)

    # print("in-bytes #{}, out-bytes #{}".format(len(test_data), len(encoded_bytes)))
    return encoded_bytes


def test_1byte():
    dummy_data = list([65])
    compressed_data = _common_enc_dec(dummy_data)
    return compressed_data


def test_2bytes():
    dummy_data = list([1, 2])
    compressed_data = _common_enc_dec(dummy_data)
    return compressed_data


def test_all_zeros():
    dummy_data = list([0]*24)
    compressed_data = _common_enc_dec(dummy_data)
    return compressed_data


def test_all_equal_non_zero():
    dummy_data = list([64]*64)
    compressed_data = _common_enc_dec(dummy_data)
    return compressed_data


def test_all_max():
    dummy_data = list([127]*64)
    compressed_data = _common_enc_dec(dummy_data)
    return compressed_data


def test_example_case1():
    dummy_data = [1, 2, 3]
    compressed_data = _common_enc_dec(dummy_data)
    # print(compressed_data)
    return compressed_data


def test_example_case2():
    dummy_data = [1, 2, 2]
    compressed_data = _common_enc_dec(dummy_data)
    # print(compressed_data)
    return compressed_data


def test_example_case3():
    dummy_data = [1, 2, 2, 3]
    compressed_data = _common_enc_dec(dummy_data)
    # print(compressed_data)
    return compressed_data


def test_example_case4():
    dummy_data = [1, 2, 2, 3, 3, 3]
    compressed_data = _common_enc_dec(dummy_data)
    # print(compressed_data)
    return compressed_data


def test_concatenated_cases():
    dummy_data = [1, 2, 3] + \
                 [1, 2, 2] + \
                 [1, 2, 2, 3] + \
                 [1, 2, 2, 3, 3, 3]

    x = test_example_case1() + test_example_case2() + test_example_case3() + test_example_case4()
    y = encoder(dummy_data)

    if COMPRESSOR_ID == "THE NIBBLER":
        assert x != y       # cant guarantee that packing encoded bytes will be same as encoded a subset.

    if COMPRESSOR_ID == "THE DUPE":
        assert x == y       # can guarantee that they will be the same.

    assert decoder(x) == dummy_data
    assert decoder(y) == dummy_data


def test_monotonic_inc():
    def f(x): return x + randint(0, 1)
    dummy_data = [f(x) for x in range(0, 100)]
    compressed_data = _common_enc_dec(dummy_data)


def test_monotonic_dec():
    def f(x): return x - randint(0, 1)
    dummy_data = [f(x) for x in range(100, 0, -1)]
    # dummy_data = [ 9,  8,  8,  6,  5,  5,  3,  2,  1,  0]
    compressed_data = _common_enc_dec(dummy_data)


def test_noise_uniform():
    """ test_noise_uniform: Random data should not be compressible, but should be lossless decompressed."""
    dummy_data = np.array(np.random.uniform(0, 1, 100) * 127, dtype=np.uint8)

    encoded_bytes = encoder(dummy_data)
    decoded_bytes = decoder(encoded_bytes)

    assert list(decoded_bytes) == list(dummy_data)


def test_example_data():
    example_data = [0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
                    0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09]

    compressed_data = _common_enc_dec(example_data)
    assert len(compressed_data) <= 16
    return compressed_data


def test_image_with_variable_length():
    """ Test if an encoding data stream can be concatenated with same results on 'real' data"""
    img_path = os.path.abspath(os.path.join(__file__, '../', 'test_data', 'falcon-heavy-test-flight-spacex.png'))
    img = mpimg.imread(img_path)
    c = (img[:, :, 0] + img[:, :, 1] + img[:, :, 2]) / 3
    cu8 = np.array(c / np.max(c) * 127, dtype=np.uint8)

    rows, cols = np.shape(cu8)
    cu8_compressed = np.zeros((rows, cols), dtype=np.uint8)
    bytes_compressed = 0
    row_encoder_len = np.zeros(cols)    # since zero padding not included storing each rows length for decompress input

    def encode_section(row, start, stop):
        encoded_bytes = encoder(cu8[row, start:stop])
        i = int(row_encoder_len[row])
        cu8_compressed[row, i: i+len(encoded_bytes)] = encoded_bytes
        row_encoder_len[row] += len(encoded_bytes)

        return row_encoder_len[row]

    for row in range(0, rows):
        m = randint(0, cols)    # choose a random point to split the row and encode [0:m] and [m+1:cols]
        bytes_compressed += encode_section(row, 0, m)
        bytes_compressed += encode_section(row, m, cols)

        decoded_bytes = decoder(cu8_compressed[row, 0: int(row_encoder_len[row])])
        assert list(cu8[row, :]) == list(decoded_bytes)


def test_image_and_save():
    img_path = os.path.abspath(os.path.join(__file__, '../', 'test_data', 'falcon-heavy-test-flight-spacex.png'))
    img = mpimg.imread(img_path)
    c = (img[:, :, 0] + img[:, :, 1] + img[:, :, 2]) / 3
    cu8 = np.array(c / np.max(c) * 127, dtype=np.uint8)

    rows, cols = np.shape(cu8)
    cu8_compressed = np.zeros((rows, cols), dtype=np.uint8)
    bytes_compressed = 0
    row_encoder_len = []    # since zero padding not included storing each rows length for decompress input

    for row in range(0, rows):
        encoded_bytes = encoder(cu8[row, :])
        bytes_compressed += len(encoded_bytes)
        row_encoder_len.append(len(encoded_bytes))
        cu8_compressed[row, 0: len(encoded_bytes)] = encoded_bytes

        decoded_bytes = decoder(encoded_bytes)
        assert list(cu8[row, :]) == list(decoded_bytes)

    cu8_uncompressed = np.zeros((rows, cols), dtype=np.uint8)
    for row in range(0, rows):
        encoded_bytes = cu8_compressed[row, 0:row_encoder_len[row]]
        decoded_bytes = decoder(encoded_bytes)
        cu8_uncompressed[row, :] = decoded_bytes[0:cols]

    plt.figure()
    plt.subplot(221)
    plt.title("Input Image {} bytes".format(rows*cols))
    plt.imshow(cu8)
    plt.axis('off')

    plt.subplot(222)
    plt.title("Compressed Image {} bytes".format(bytes_compressed))
    plt.imshow(cu8_compressed)
    plt.axis('off')

    plt.subplot(224)
    plt.title("Decompressed Image ".format(bytes_compressed))
    plt.imshow(cu8_uncompressed)
    plt.axis('off')
    img_out_path = os.path.abspath(os.path.join(__file__, '../', 'test_data', 'compressed-falcon-heavy-spacex.png'))
    plt.savefig(img_out_path)
    # plt.show()
