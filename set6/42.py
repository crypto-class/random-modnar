#!/usr/bin/env python
# coding: utf-8

'''
Bleichenbacher's e=3 RSA Attack
一种低指数攻击情形，可以用来RSA签名伪造.
'''
import gmpy2
from Crypto.Util.number import getPrime
from hashlib import sha1
from decimal import Decimal as D
from decimal import getcontext

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

# 按照pkcs1.5进行填充
def pkcs_1_padding(msg, total_len):
    pad_num = total_len - 2 - 20 - 1 - 10
    msg = '\x00\x01' + pad_num * '\xff' + '\x00' + msg
    '''
    0x1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00925a89b43f3caff507db0a86d20a2428007f10b6L
    '''
    # print hex(a2i(msg))
    return msg
# 按照pkcs1.5进行解析，提取出签名

'''
Here use a wrong way to extract the signature and verify.
'''
def pkcs_1_unpadding(msg):
    if msg[0] == '\x00' and msg[1] == '\x01':
        # From index 2 to find the second '\x00' bit, output the location.
        second  = msg.find('\x00', 2)
        # from second+1 to second +21 is the ASN.1 GOOP HASH
        return msg[second + 1 : second + 1 + 20]
    else:
        return None

# 这里的Key用的公钥
# Only when e == 3 this sign_fake method can use
def sign_fake(msg, key):

    dgst = sha1(msg).digest()
    # len(dgst) = 20
    keylen = len(i2a(key[1]))
    # set precision
    getcontext().prec = keylen * 8
    f_sign     = '\x00\x01' + 8 * '\xff' + '\x00' + dgst
    f_message  = (keylen - 2 - 8 - 1 - len(dgst) - 10) * '\x00'
    f_sign_num = a2i(f_sign + f_message)
    # f_sign_num^(1/3)
    '''
    0x1ffffffffffffffff00925a89b43f3caff507db0a86d20a2428007f10b60000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000L
    '''
    # print hex(f_sign_num)
    f_sign = int(pow(f_sign_num, D(1)/D(3)))+1
    return f_sign


# 这里的Key用的是私钥才合理
def rsa_sign_make(msg, key):

    dgst      = sha1(msg).digest()
    total_len = len(i2a(key[1]))
    # total_len = 256
    pad_dgst  = pkcs_1_padding(dgst, total_len)

    return rsa_enc(pad_dgst, key)


# key use the public !
def rsa_sign_verify(msg, sign, key):

    dgst   = sha1(msg).digest()
    string = '\x00' + rsa_dec(sign, key)
    print hex(a2i(string))
    '''
    1:
    0x1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00925a89b43f3caff507db0a86d20a2428007f10b6L
    2:
    0x1ffffffffffffffff00925a89b43f3caff507db0a86d20a2428007f10b60000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000063884ddf237c677d447214c2af8cf3b3e151969e166477d3174d614d6e3ef0ebb6885f2903ef916d92d800546a18b7a7f9dc45592e2098ed5d5d08142274bb4431b7cc8aadae7a1c0b2afc84f729ce070fced200c6e992aba3802827e145d958242779fc4bf6e6c5fd039c64e5f02bc1af801b5444660df4c6329b44b6add33765429dbbc9663675085c649a2f30f4638d0246900cc96c2e3bab75af025001ef615678L

    '''
    if dgst == pkcs_1_unpadding(string):
        return True
    else:
        return False

if __name__ == "__main__":

    message = "hi mom"

    print "Message is: " + message

    pub, prv = rsa_make_keypair()

    sign = rsa_sign_make(message, prv)
    print "RSA sign is: " + i2a(sign)

    if rsa_sign_verify(message, sign, pub) == True:
        print "Valid Signatrue!"
    else :
        print "Invalid Signatrue!"

    f_sign = sign_fake(message, pub)
    print "f_sign is: " + i2a(f_sign)
    if rsa_sign_verify(message, f_sign, pub) == True:
        print "Valid Signatrue!"
    else :
        print "Invalid Signatrue!"
