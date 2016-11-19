#!/usr/bin/python

'''
File name: main.py
Created by: WRH
Email: o@seceve.com
http://www.cryptopals.com/sets/4/challenges/27
'''



length = 16

pad = lambda s: s + (16 - len(s) % 16) * chr(16 - len(s) % 16)
unpad = lambda s : s[0:-ord(s[-1])]

def gen_key():
    from random import randint,choice
    seed = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+=-"
    k = ''
    for i in range(0,16):
        k += choice(seed)
    return k

_key = gen_key()
print 'iv(key)      : ' + _key

def xor(str1, str2): 
    assert len(str1) == 16
    assert len(str2) == 16
    r = ''
    for i in range(0,16):
        r += chr(ord(str1[i]) ^ ord(str2[i]))
    return r

#Using the key as an IV is insecure; an attacker that can modify ciphertext in flight can get the receiver to decrypt a value that will reveal the key.
def encrypt(data):
    from Crypto.Cipher import AES
    cipher = AES.new(_key,AES.MODE_CBC,_key)
    return cipher.encrypt(pad(data))

def decrypt(data): 
    from Crypto.Cipher import AES
    cipher = AES.new(_key,AES.MODE_CBC,'\x00'*16) # Just block cipher decryption
    plaintext = ''
    cipertext_block = ''
    iv = _key
    for i in range(0,len(data)/16): # For every block:
        cipertext_block = data[16*i:16*i+16]
        plaintext_block = cipher.decrypt(cipertext_block)
        plaintext += xor(plaintext_block,iv) # raise the appropriate error if high-ASCII is found.
        iv = cipertext_block
    return plaintext


# Use your code to encrypt a message that is at least 3 blocks long:
plaintext = '0123456789abcdef'*3                       # P_1, P_2, P_3
ciphertext = encrypt(plaintext)                        # AES-CBC(P_1, P_2, P_3) -> C_1, C_2, C_3
# Modify the message (you are now the attacker):
attack_ciphertext = ciphertext[:16] +\
    '\x00' * 16 + ciphertext[:16]                      # C_1, C_2, C_3 -> C_1, 0, C_1
attack_plaintext = decrypt(attack_ciphertext)          # P'_1 P'_2 P'_3
# Decrypt the message (you are now the receiver) and raise the appropriate error if high-ASCII is found.
# As the attacker, recovering the plaintext from the error, extract the key:
print 'Attacker Key : ' +\
    xor(attack_plaintext[:16],attack_plaintext[32:48]) # P'_1 XOR P'_3



