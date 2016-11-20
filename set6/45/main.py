#!/usr/bin/python

'''
File name: main.py
Created by: WRH
Email: o@seceve.com
http://www.cryptopals.com/sets/4/challenges/45
'''

from gmpy2 import invert
from hashlib import sha1
from random import randint
def H(message):
     return sha1(message).hexdigest()

p = 0x800000000000000089e1855218a0e7dac38136ffafa72eda7859f2171e25e65eac698c1702578b07dc2a1076da241c76c62d374d8389ea5aeffd3226a0530cc565f3bf6b50929139ebeac04f48c3c84afb796d61e5a4f9a8fda812ab59494232c7d2b4deb50aa18ee9e132bfa85ac4374d7f9091abc3d015efc871a584471bb1
q = 0xf4f47f05794b256174bba6e9b396a7707e563c5b
g = 0x5958c9d3898b224b12672c0b98e06c60df923cb8bc999d119458fef538b8fa4046c8db53039db620c094c9fa077ef389b5322a559946a71903f990f1f7e0e025e2d7f7cf494aff1a0470f5b64c36b625a097f1651fe775323556fe00b3608c887892878480e99041be601a62166ca6894bdd41a7054ec89f756ba9fc95302291


x = 97856928
y = pow(g,x,p)
g = p + 1

# For arbitrary z:
z = randint(1,100)
# # r = ((y**z) % p) % q
r = ((y**z)%p % q)
#        r
#  s =  --- % q
#        z
s = (invert(z,q) * r) % q

message1 = 'Hello, world'
message2 = 'Goodbye, world'

# Sign "Hello, world". And "Goodbye, world".

hash_1 = int(H(message1),16)
hash_2 = int(H(message2),16)

def validate(message):
    w = invert(s,q)
    u1 = (int(H(message),16) * w) % q
    u2 = (r * w) % q
    v = (pow(g,u1,p) * pow(y,u2,p)) % q
    return v == r

# validate
print 'Validate Hello, world   : ' + str(validate(message1))
print 'Validate Goodbye, world : ' + str(validate(message2))




