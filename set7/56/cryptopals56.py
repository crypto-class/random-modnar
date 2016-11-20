# coding=utf-8

import os
import time
from rc4 import rc4

secret_length = 30

def padding(req):
    key = os.urandom(16)
    cookie = 'QkUgU1VSRSBUTyBEUklOSyBZT1VSIE9WQUxUSU5F'.decode('base64')

    return rc4(key).encrypt(req + cookie)

def recover(counts):
    msg = ''
    for byte in counts:
        highest_index = 0
        highest_score = 0
        for i, score in enumerate(byte):
            if score > highest_score:
                highest_score = score
                highest_index = i
        msg += chr(highest_index)
    return msg

def recover_cookie(f):
    zero = [0] * 256
    counts = [list(zero) for i in range(secret_length)]

    FULL_WEIGHT = 4
    HALF_WEIGHT = 1

    prefix = 'AA'
    while len(prefix) <= 17:
        lp = len(prefix)
        for r in xrange(2 ** 16):
            ct = f(prefix)

            b16 = ord(ct[15])
            byte = counts[15 - lp]
            byte[b16 ^ 240] += FULL_WEIGHT
            byte[b16 ^ 0]   += HALF_WEIGHT
            byte[b16 ^ 16]  += HALF_WEIGHT

            b32 = ord(ct[31])
            byte = counts[31 - lp]
            byte[b32 ^ 224] += FULL_WEIGHT
            byte[b32 ^ 0]   += HALF_WEIGHT
            byte[b32 ^ 32]  += HALF_WEIGHT

        print "%17s : "%(prefix),
        print recover(counts).encode("hex")
        prefix += 'A'

if __name__ == '__main__':
    time.clock()
    recover_cookie(padding)
    print "spend time:",time.clock()