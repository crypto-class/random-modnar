#!/usr/bin/env python
# coding: utf-8

'''
Implement unpadded message recovery oracle
实现未填充的消息的恢复情形，例如Javascript.
'''
# gmpy2 supports GMP(multiple-precision) lib, MPFR, MPC libs and so on.
import gmpy2
#import os
#import binascii
from Crypto.Util.number import  getPrime
import random

#def find_prime(bytez = 256):
#    p = int(binascii.hexlify(os.urandom(bytez)), 16) | 1
#    while not gmpy2.is_bpsw_prp(p):
#        p = int(binascii.hexlify(os.urandom(bytez)), 16) | 1
#    return p

def make_rsa_keypair():
    bits = 1024
#    p, q = find_prime(), find_prime()  # This method is slower than below.
    p,q = gmpy2.mpz(getPrime(bits)), gmpy2.mpz(getPrime(bits))
    N   = gmpy2.mpz(p * q)
    phi = (p - 1) * (q - 1)
    e   = 0x10001
    d   = gmpy2.invert(e, phi)
    if d == None:
        make_rsa_keypair()
    return (e, N), (d, N)
def rsa_enc(m, (e, N)):

    # transform ASCII to int.
    m = int(m.encode('hex'), 16)


    len_m = gmpy2.bit_length(m) - 1
    len_n = gmpy2.bit_length(N) - 1
    if len_n < len_m: # if message is bigger than N
        pass
    else:
        c = gmpy2.powmod(m, e, N)
        return c

def rsa_dec(c, (d, N)):
    m = gmpy2.powmod(c, d, N)

    # transform int to ASCII.
    m = hex(m).strip('0xXlL')
    if len(m) % 2 == 1:
        m = '0' + m
    return m.decode('hex')

if __name__ == "__main__":
    pub, prv = make_rsa_keypair()
    print 'The original message is: '
    m = "Hello, I'm fine!"

    Cipher = rsa_enc(m, pub)
    #print Cipher

    if rsa_dec(Cipher, prv) != m:
        print "Wrong~ "
    print rsa_dec(Cipher, prv)

    # Assume that now we capture the ciphertext m.

    # Here we can choose a integer > 1 as S and can not use 2*2*2*2*...

    S = 3
    print "The S I chose is: " + str(S)
    # We can change our captured cipher using S

    # m' = S ^ e % N
    Cipher_mod  = gmpy2.powmod(S, pub[0], pub[1])
    # C' = (C * m') mod N
    Cipher_mod  = gmpy2.f_mod(gmpy2.mul(Cipher, Cipher_mod), pub[1])
    # M' = E(C')
    Message_mod = rsa_dec(Cipher_mod, prv)

    #transform ASCII to int
    Message_num = int(Message_mod.encode('hex'), 16)

    # The most important step :
    Message_num = gmpy2.f_mod(gmpy2.mul(gmpy2.mpz(Message_num), gmpy2.invert(S, pub[1])), pub[1])
    # transform int to ASCII.
    Message     = hex(Message_num).strip('0xXlL')
    if len(Message) % 2 == 1:
        Message = '0' + Message
    print 'The message we recovered: '
    print Message.decode('hex')
