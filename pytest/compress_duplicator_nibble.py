"""
    Users  compress_duplicator.py but instead of storing the duplicate count as byte, store it as a nibble.

    Iterate over the data and when a duplicate value is encountered, set the MSB on that value marking it as a duplicate
    and add a NIBBLE containing the number of repetition which follows it. This restricts the number of duplicates that
    can be encoded per MSB to 17 sequential values (2 + MAX_SIZE(NIBBLE)).

    Other restrictions include:
        Random uniform data may result in output being larger.
        Data lengths smaller than 4 bytes are also not guaranteed to fit inplace. :(

    Analysis: Best case is when every 18th data point is not a duplicate.
              Worst case is when there are no duplicates.
              If worst cases occurs often consider alternative method like delta compression.
"""

import numpy as np
from compress_duplicator import encoder_duplicates, decoder_duplicates

COMPRESSOR_ID = "THE NIBBLER"


def __format(num, size=8):
    """Format a number as binary with leading zeros"""
    return str(bin(num)[2::]).zfill(size)


def nibbles_unpack(byte_value):
    upper = (byte_value & 0xF0) >> 4
    lower = (byte_value & 0x0F) >> 0
    return [upper, lower]


END_BYTE = __format(127 + 128)    # can be chosen to be any byte with value greater than 128,
                                  # best choice value which occurs least frequently in data.


def encoder_duplicates_with_nibble(data):
    encode_duplicates = encoder_duplicates(data, max_duplicates=15, exclude=127)

    # pack duplicate counts as nibbles in sequence: python shortcut convert to str sequence and make binary
    packed_bits = ""
    _skip_iter = False
    for i, v in enumerate(encode_duplicates):
        if _skip_iter:
            _skip_iter = False
            continue

        packed_bits += __format(v, 8)

        if v >= 128:  # detect msb set, which means next value is a duplicate count results
            v_next = encode_duplicates[i+1]
            packed_bits += __format(v_next, 4)
            _skip_iter = True

    if len(packed_bits) % 8 != 0:  # ends with a nibble so zero pad full byte
        packed_bits += END_BYTE    # on decode search for END_BYTE and ignore values nibble after it.
        packed_bits += "0000"

    # binary string pack into int8 array
    encode_duplicate_nibbles = []
    j = 0
    while j < len(packed_bits):
        v = int(packed_bits[j:j+8], 2)
        encode_duplicate_nibbles.append(v)
        j += 8

    encode_duplicate_nibbles = np.array(encode_duplicate_nibbles, dtype=np.uint8)
    return list(encode_duplicate_nibbles)


def decoder_duplicates_with_nibble(data):
    # first need to unpack int8 data into binary string to convert duplicate count nibbles as bytes.
    packed_bits = ""
    nibble_pad = "0000"

    # convert to binary string
    for i, v in enumerate(data):
        packed_bits += __format(v)

    unpack_bytes = []
    nibble_found = False
    j = 0
    while j < len(packed_bits):
        if nibble_found:
            v = int(nibble_pad + packed_bits[j: j+4], 2)
            unpack_bytes.append(v)
            j += 4
            nibble_found = False
        else:
            v = int(packed_bits[j: j+8], 2)
            if v != int(END_BYTE, 2):
                unpack_bytes.append(v)

                j += 8
                nibble_found = False if v < 128 else True
            else:
                j += 12     # Skip the END_BYTE and the zfill nibble, continue if concatenated encoded bytes.

    return decoder_duplicates(unpack_bytes)


if __name__ == "__main__":
    example_data = [0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
                    0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09]

    print("Input Bytes {} : {}". format(len(example_data), example_data))

    encoded_bytes = encoder_duplicates_with_nibble(example_data)
    print("Encoded Bytes {} : {}". format(len(encoded_bytes), encoded_bytes))

    if len(example_data) > 4:
        assert len(encoded_bytes) <= len(example_data)

    decoded_bytes = decoder_duplicates_with_nibble(encoded_bytes)
    print("Decoded Bytes {} : {}". format(len(decoded_bytes), decoded_bytes))

    assert list(decoded_bytes) == list(example_data)
