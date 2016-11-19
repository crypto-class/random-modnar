#!/usr/bin/env python
# coding: utf-8

'''
RSA parity oracle
玩具式的攻击方法，采用了二分的思想进行明文的恢复
'''
import gmpy2
from Crypto.Util.number import getPrime
from hashlib import sha1
from decimal import Decimal as D
from decimal import getcontext
import base64
import math
import datetime

# Here use the gcd() to judge the invert a^-1(mod b) existance
def gcd(a, b):
    while b:
        a, b = b, a%b
    return a

def rsa_make_keypair():
    bits = 512   # 1024/2
    e = 0x3
    while True:
        p,q = gmpy2.mpz(getPrime(bits)), gmpy2.mpz(getPrime(bits))
        N   = gmpy2.mpz(p * q)
        phi = (p - 1) * (q - 1)
        if gcd(e, phi) == 1:
            break
    #print e, phi
    d   = gmpy2.invert(e, phi)

    return (e, N), (d, N)
def rsa_enc(m, (e, N)):

    m     = a2i(m)
    len_m = gmpy2.bit_length(m) - 1
    len_n = gmpy2.bit_length(N) - 1
    if len_n < len_m: # if message is bigger than N
        pass
    else:
        c = gmpy2.powmod(m, e, N)
        return c

def rsa_dec(c, (d, N)):
    m = gmpy2.powmod(c, d, N)
    return i2a(m)

def a2i(m):
    # transform ASCII to int.
    return int(m.encode('hex'), 16)
def i2a(i):
    # transform int to ASCII.
    x = hex(i).strip('0xXlL')
    if len(x) % 2 == 1:
        x = '0' + x
    return x.decode('hex')


def binary_attack(encrypt, key1, key2):
    S = gmpy2.powmod(2, key1[0], key1[1])
    encrypt = encrypt * S % key1[1];
    lo = D(0)
    up = D(int(key1[1]))
    bound = int(gmpy2.log(key1[1])/gmpy2.log(2)) + 1
    getcontext().prec = bound
    for x in range(bound):
        # each time determine the most significant digit.
        middle = (lo+up)/2
        # oracle function
        if gmpy2.powmod(encrypt, key2[0], key2[1]) % 2 == 0:
            up = middle
        else:
            lo = middle
        encrypt = encrypt * S % key1[1]
    print "The original message is: "
    # print the upper_bound
    print i2a(int(up)).encode('utf-8')
if __name__ == "__main__":
    print 'Wait please, it will take about 3 secs to finish...'
    #set a timer to count
    #start_time = datetime.datetime.now()
    msg = base64.b64decode("VGhhdCdzIHdoeSBJIGZvdW5kIHlvdSBkb24ndCBwbGF5IGFyb3VuZCB3aXRoIHRoZSBGdW5reSBDb2xkIE1lZGluYQ==")
    # That's why I found you don't play around with the Funky Cold Medina
    pub, prv = rsa_make_keypair()
    encrypt = rsa_enc(msg, pub)
    binary_attack(encrypt, pub, prv)
    #end_time = datetime.datetime.now()
    #print (end_time - start_time).seconds
