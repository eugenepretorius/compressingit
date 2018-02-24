"""
    A simple data compressor to remove sequential duplicates in data stream.
    Without making any assumptions about the data distributions, or storing any tables too reduces overhead size.

    Data input: array(data, dtype=np.int8)
    Data range: 0 - 127
    Data is unstructured, and frequently contains sequential duplicated values.

    Algorithm description
    ---------------------

    Iterate over the data and when a duplicate value is encountered, set the MSB on that value marking it as a duplicate
    and add a byte containing the number of repetition which follows it.

    Example:
        CASE 1: [ 1, 2, 3] -> [1, 2, 3]
        CASE 2: [ 1, 2, 2] -> [1, 130, 0]
        CASE 3: [ 1, 2, 2, 3] -> [1, 130, 0, 3]
        CASE 4: [ 1, 2, 2, 3, 3, 3] -> [1, 130, 0, 131, 1]

    Analysis: Best case is when all data is duplicates. Worst case is when there are no duplicates.
              If worst cases occurs often consider delta compression prior to this setup.

    Pros: If will never return an encoding size larger than the input.
          Its simple.
          It can be implemented as an in-place operation.
          It fits into a function, and does not require external functions.
    Cons: It could regularly return the same size if no sequential duplicates are encountered.
          Such cases where data is monotonically increasing or decreasing will not perform well.

"""

import numpy as np

COMPRESSOR_ID = "THE DUPE"


def encoder_duplicates(data, max_duplicates=127, exclude=None):
    """
    Encodes a data stream using simple RLE.
    :param data: numpy.array(data, dtype=np.int8)
    :param max_duplicates: restrict the number of duplicates that can be counted and packed.
    :param exclude: exclude this value from being encoded
    :return: list of encoded values.
    """
    assert max_duplicates <= 127     # msb values are part of encoding
    data = np.array(data, dtype=np.int8)

    encode_duplicates = []
    cnt_duplicates = 0
    for i, v in enumerate(data):
        if cnt_duplicates > 0:
            cnt_duplicates -= 1
            continue

        if i+1 < len(data):
            v_next = data[i+1]
        else:
            v_next = -1

        if v == v_next and v != exclude:
            cnt_duplicates = 1
            for j in range(i+2, len(data)):
                if v == data[j]:
                    cnt_duplicates += 1
                    if cnt_duplicates >= max_duplicates:
                        break
                else:
                    break
            encode_duplicates.append(v+128)
            encode_duplicates.append(cnt_duplicates-1)
        else:
            encode_duplicates.append(v)

    encode_duplicates = np.array(encode_duplicates, dtype=np.uint8)
    return list(encode_duplicates)


def decoder_duplicates(data):
    x = []
    _skip = False
    for i, v in enumerate(data):
        if _skip:
            _skip = False
            continue

        if i + 1 < len(data):
            v_next = data[i+1]
        else:
            v_next = -1

        if v >= 128:
            x.append(v - 128)
            if v_next != -1 and v_next < 128:
                for j in range(0, v_next+1):
                    x.append(v - 128)
                _skip = True
            else:
                x.append(v - 128)
        else:
            x.append(v)

    return x


if __name__ == "__main__":
    example_data = [0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
                    0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09]

    # example_data = list(range(24))
    # example_data = list([0]*24)
    # example_data = [ 1, 2, 2]

    encoded_bytes = encoder_duplicates(example_data) + encoder_duplicates(example_data)
    print("Encoded Bytes {} : {}". format(len(encoded_bytes), encoded_bytes))

    # assert len(encoded_bytes) <= len(example_data)

    decoded_bytes = decoder_duplicates(encoded_bytes)
    print("Decoded Bytes {} : {}". format(len(decoded_bytes), decoded_bytes))

    assert list(decoded_bytes) == list(example_data) + list(example_data)


