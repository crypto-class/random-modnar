#!/usr/bin/python

'''
File name: main.py
Created by: WRH
Email: o@seceve.com
http://www.cryptopals.com/sets/4/challenges/30
'''

# reference https://github.com/josephw/python-md4/blob/master/md4.py

# Second verse, same as the first, but use MD4 instead of SHA-1. Having done this attack once against SHA-1, the MD4 variant should take much less time; mostly just the time you'll spend Googling for an implementation of MD4.

from struct import pack
from binascii import hexlify

def make_words(byte_array):
    res = []
    for i in xrange(0, len(byte_array), 4):
        index = i/4
        res.append(byte_array[i+3])
        res[index] = (res[index] << 8) | byte_array[i+2]
        res[index] = (res[index] << 8) | byte_array[i+1]
        res[index] = (res[index] << 8) | byte_array[i]
    return res

def mymd4(message,original_length = -1,ABCD = [0x67452301,0xefcdab89,0x98badcfe,0x10325476]):
    if original_length == -1: original_length = len(message)
    message = [ord(c) for c in message]
    message += [0x80] + [0x00] * ((56 - (len(message)+1)%64)%64)
    message += [ord(c) for c in pack('<Q', (original_length * 8))]
    A,B,C,D = ABCD
    # define F, G, and H
    def F(x,y,z): return ((x & y) | ((~x) & z))
    def G(x,y,z): return (x & y) | (x & z) | (y & z)
    def H(x,y,z): return x ^ y ^ z
    # round functions
    def FF(a,b,c,d,k,s): return ROL((a + F(b,c,d) + X[k]) & 0xFFFFFFFF, s)
    def GG(a,b,c,d,k,s): return ROL((a + G(b,c,d) + X[k] + 0x5A827999) & 0xFFFFFFFF, s)
    def HH(a,b,c,d,k,s): return ROL((a + H(b,c,d) + X[k] + 0x6ED9EBA1) & 0xFFFFFFFF, s)
    # define a 32-bit left-rotate function (<<< in the RFC)
    def ROL(x, n): return ((x << n) & 0xFFFFFFFF) | (x >> (32-n))
    # turn the padded message into a list of 32-bit words
    M = make_words(message)  
    # process each 16 word (64 byte) block
    for i in xrange(0, len(M), 16):
        X = M[i:i+16]
        AA,BB,CC,DD = A,B,C,D
        # round 1
        A = FF(A,B,C,D,0,3)  ; D = FF(D,A,B,C,1,7)  ; C = FF(C,D,A,B,2,11)  ; B = FF(B,C,D,A,3,19)
        A = FF(A,B,C,D,4,3)  ; D = FF(D,A,B,C,5,7)  ; C = FF(C,D,A,B,6,11)  ; B = FF(B,C,D,A,7,19)
        A = FF(A,B,C,D,8,3)  ; D = FF(D,A,B,C,9,7)  ; C = FF(C,D,A,B,10,11) ; B = FF(B,C,D,A,11,19)
        A = FF(A,B,C,D,12,3) ; D = FF(D,A,B,C,13,7) ; C = FF(C,D,A,B,14,11) ; B = FF(B,C,D,A,15,19)
        # round 2
        A = GG(A,B,C,D,0,3)  ; D = GG(D,A,B,C,4,5)  ; C = GG(C,D,A,B,8,9)   ; B = GG(B,C,D,A,12,13)
        A = GG(A,B,C,D,1,3)  ; D = GG(D,A,B,C,5,5)  ; C = GG(C,D,A,B,9,9)   ; B = GG(B,C,D,A,13,13)
        A = GG(A,B,C,D,2,3)  ; D = GG(D,A,B,C,6,5)  ; C = GG(C,D,A,B,10,9)  ; B = GG(B,C,D,A,14,13)
        A = GG(A,B,C,D,3,3)  ; D = GG(D,A,B,C,7,5)  ; C = GG(C,D,A,B,11,9)  ; B = GG(B,C,D,A,15,13)
        # round 3
        A = HH(A,B,C,D,0,3)  ; D = HH(D,A,B,C,8,9)  ; C = HH(C,D,A,B,4,11)  ; B = HH(B,C,D,A,12,15)
        A = HH(A,B,C,D,2,3)  ; D = HH(D,A,B,C,10,9) ; C = HH(C,D,A,B,6,11)  ; B = HH(B,C,D,A,14,15)
        A = HH(A,B,C,D,1,3)  ; D = HH(D,A,B,C,9,9)  ; C = HH(C,D,A,B,5,11)  ; B = HH(B,C,D,A,13,15)
        A = HH(A,B,C,D,3,3)  ; D = HH(D,A,B,C,11,9) ; C = HH(C,D,A,B,7,11)  ; B = HH(B,C,D,A,15,15)
        A,B,C,D = ((A + AA) & 0xFFFFFFFF),((B + BB) & 0xFFFFFFFF),((C + CC) & 0xFFFFFFFF),((D + DD) & 0xFFFFFFFF)
    A,B,C,D = hexlify(pack('<L', A)),hexlify(pack('<L', B)),hexlify(pack('<L', C)),hexlify(pack('<L', D))
    return A + B + C + D

# A B C D

def gen_key():
    from random import randint,choice
    seed = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+=-"
    k = ''
    for i in range(0,randint(8,32)):
        k += choice(seed)
    return k

k = gen_key() # Only prefix_md4() use k
s = 'comment1=cooking%20MCs;userdata=foo;comment2=%20like%20a%20pound%20of%20bacon'
e = ';admin=true'

from Crypto.Hash.MD4 import new
def prefix_md4(data):
    return new(k+data).hexdigest()

# A B C D
ks = prefix_md4(s)
ABCD = []
for i in range(0,len(ks)/8):
    ABCD.append(int(ks[8*i+ 6:8*i+ 8]+\
                    ks[8*i+ 4:8*i+ 6]+\
                    ks[8*i+ 2:8*i+ 4]+\
                    ks[8*i+ 0:8*i+ 2],16)) # '12 34 56 78' -> '78 56 34 12'

# guess length
import struct
for key_length in range(1,32):
    length = key_length + len(s)
    glue = b'\x80' + b'\x00' * ((56 - (length + 1)%64)%64)
    length *= 8
    glue += struct.pack(b'<Q', length)
    if prefix_md4(s + glue + e) == mymd4(e,length/8 + len(glue+e),ABCD):
        print 'key length  : '+str(key_length)
        print 'prefix_md4  : '+prefix_md4(s + glue + e)
        print 'mymd4       : '+mymd4(e,length/8 + len(glue+e),ABCD)
        break







