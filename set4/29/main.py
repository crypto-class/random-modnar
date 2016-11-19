#!/usr/bin/python

'''
File name: main.py
Created by: WRH
Email: o@seceve.com
http://www.cryptopals.com/sets/4/challenges/29
'''


# SHA1
import struct
import io

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

h5 = (0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,0xC3D2E1F0,)
digest_size = 20
block_size = 64

def mysha1(data,h = h5,data_length = -1):
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
    if data_length != -1:
        process += struct.pack(b'>Q', data_length)             # 56 + 8 = 64
    else:
        process += struct.pack(b'>Q', length)
    if len(process) == 64:                                     # 64 or 128
        h = _process_chunk(process, *h)
    else:
        h = _process_chunk(process[:64], *h)
        h = _process_chunk(process[64:], *h)
    return '%08x%08x%08x%08x%08x' % h


from hashlib import sha1

# # TEST SHA1
# test_str = 'as35dgasiu6dnp2190'
# if sha1(test_str).hexdigest() == mysha1(test_str):
#     print 'TEST OK'
# else:
#     exit(0)

def gen_key():
    from random import randint,choice
    seed = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+=-"
    k = ''
    for i in range(0,randint(8,32)):
        k += choice(seed)
    return k


k = gen_key()
s = 'comment1=cooking%20MCs;userdata=foo;comment2=%20like%20a%20pound%20of%20bacon'
e = ';admin=true'

# Secret-prefix SHA-1 MACs are trivially breakable.
# The attack on secret-prefix SHA1 relies on the fact that you can take the ouput of SHA-1 and use it as a new starting point for SHA-1, thus taking an arbitrary SHA-1 hash and "feeding it more data".
# Since the key precedes the data in secret-prefix, any additional data you feed the SHA-1 hash in this fashion will appear to have been hashed with the secret key.
# To carry out the attack, you'll need to account for the fact that SHA-1 is "padded" with the bit-length of the message; your forged message will need to include that padding. We call this "glue padding". The final message you actually forge will be:
def prefix_sha1(data):
    return sha1(k+data).hexdigest()

# h
ks = prefix_sha1(s)
h = []
for i in range(0,len(ks)/8):
    h.append(int(ks[8*i:8*i+8],16))

# guess length
for key_length in range(1,32):
    length = key_length + len(s)
    glue = b'\x80' + b'\x00' * ((56 - (length + 1)%64)%64)
    length *= 8
    glue += struct.pack(b'>Q', length)
    # SHA1(key || original-message || glue-padding || new-message)
    # Forge a variant of this message that ends with ";admin=true".
    if prefix_sha1(s + glue + e) == mysha1(e,h,length + len(glue+e)*8): 
        print 'key length  : '+str(key_length)
        print 'prefix_sha1 : '+prefix_sha1(s + glue + e)
        print 'mysha1      : '+mysha1(e,h,length + len(glue+e)*8)
        break




