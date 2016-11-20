#!/usr/bin/python

'''
File name: main.py
Created by: WRH
Email: o@seceve.com
http://www.cryptopals.com/sets/4/challenges/43
'''

# H : Sha1
from hashlib import sha1
def H(message):
     return sha1(message).hexdigest()

p = 0x800000000000000089e1855218a0e7dac38136ffafa72eda7859f2171e25e65eac698c1702578b07dc2a1076da241c76c62d374d8389ea5aeffd3226a0530cc565f3bf6b50929139ebeac04f48c3c84afb796d61e5a4f9a8fda812ab59494232c7d2b4deb50aa18ee9e132bfa85ac4374d7f9091abc3d015efc871a584471bb1

q = 0xf4f47f05794b256174bba6e9b396a7707e563c5b
 
g = 0x5958c9d3898b224b12672c0b98e06c60df923cb8bc999d119458fef538b8fa4046c8db53039db620c094c9fa077ef389b5322a559946a71903f990f1f7e0e025e2d7f7cf494aff1a0470f5b64c36b625a097f1651fe775323556fe00b3608c887892878480e99041be601a62166ca6894bdd41a7054ec89f756ba9fc95302291

message = 'For those that envy a MC it can be hazardous to your health\nSo be friendly, a matter of life and death, just like a etch-a-sketch\n'

# Test H()
# print H(message) == 'd2d0714f014a9784047eaeccf956520045c45265'

msg_hash = long(H(message),16)

r = 548099063082341131477253921760299949438196259240
s = 857042759984254168557880549501802188789837994940

y = 0x84ad4719d044495496a3201c8ff484feb45b962e7302e56a392aee4abab3e4bdebf2955b4736012f21a08084056b19bcd7fee56048e004e44984e2f411788efdc837a0d2e5abb7b555039fd243ac01f0fb2ed1dec568280ce678e931868d23eb095fde9d3779191b8c0299d6e07bbb283e6633451e535c45513b2d33c99ea17

# Get x
from gmpy2 import invert,powmod
inv_r = invert(r,q)
for k in range(0,2**16):
    # Given a known "k", it's trivial to recover the DSA private key "x":
    x = (inv_r * ( s*k - msg_hash )) % q
    if powmod(g,x,p) == y:
        print sha1('%x' % x).hexdigest() # Test 0954edd5e0afe5542a4adf012611a91912a3ec16
        print 'x: ' + str(x)
        print 'k: ' + str(k)
        break


