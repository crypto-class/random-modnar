#!/usr/bin/python

'''
File name: main.py
Created by: WRH
Email: o@seceve.com
http://www.cryptopals.com/sets/4/challenges/28
'''


# SHA1
import struct
import io

# Find a SHA-1 implementation in the language you code in.
def _left_rotate(n, b):
    return ((n << b) | (n >> (32 - b))) & 0xffffffff

def _process_chunk(chunk, h0, h1, h2, h3, h4):
    assert len(chunk) == 64
    w = [0] * 80
    for i in range(16):
        w[i] = struct.unpack(b'>I', chunk[i*4:i*4 + 4])[0]
    for i in range(16, 80):
        w[i] = _left_rotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1)
    a = h0 ; b = h1 ; c = h2 ; d = h3 ; e = h4
    
    for i in range(80):
        if 0 <= i <= 19:
            f = d ^ (b & (c ^ d))
            k = 0x5A827999
        elif 20 <= i <= 39:
            f = b ^ c ^ d
            k = 0x6ED9EBA1
        elif 40 <= i <= 59:
            f = (b & c) | (b & d) | (c & d) 
            k = 0x8F1BBCDC
        elif 60 <= i <= 79:
            f = b ^ c ^ d
            k = 0xCA62C1D6
        a, b, c, d, e = ((_left_rotate(a, 5) + f + e + k + w[i]) & 0xffffffff, 
                        a, _left_rotate(b, 30), c, d)
    h0 = (h0 + a) & 0xffffffff
    h1 = (h1 + b) & 0xffffffff 
    h2 = (h2 + c) & 0xffffffff
    h3 = (h3 + d) & 0xffffffff
    h4 = (h4 + e) & 0xffffffff
    return h0, h1, h2, h3, h4


digest_size = 20
block_size = 64

# Do not use the SHA-1 implementation your language already provides (for instance, don't use the "Digest" library in Ruby, or call OpenSSL; in Ruby, you'd want a pure-Ruby SHA-1).
def mysha1(data):
    h = (0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,0xC3D2E1F0,)
    data = io.BytesIO(data)
    length = 0
    process = data.read(64)
    while 64 == len(process):
        h = _process_chunk(process, *h)
        length += 64
        process = data.read(64)
    length += len(process)
    process += b'\x80' + b'\x00' * ((56 - (length + 1)%64)%64) # 56
    length *= 8
    process += struct.pack(b'>Q', length)                      # 56 + 8 = 64
    if len(process) == 64:                                     # 64 or 128
        h = _process_chunk(process, *h)
    else:
        h = _process_chunk(process[:64], *h)
        h = _process_chunk(process[64:], *h)
    return '%08x%08x%08x%08x%08x' % h

# Write a function to authenticate a message under a secret key by using a secret-prefix MAC, which is simply:
def sha1_mac(key,data):
    return mysha1(key+data) # SHA1(key || message)

from hashlib import sha1

# TEST SHA1
key = '123214124124'
data = 'asdgasdgashdasdfasf'
if sha1(key+data).hexdigest() == sha1_mac(key,data):       # Test my sha1
    print 'TEST OK'






