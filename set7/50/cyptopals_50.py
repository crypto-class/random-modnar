# coding=utf-8

from Crypto.Cipher import AES, PKCS1_v1_5

def PKCSv7(data):
    size = 16-len(data)%16
    return data+"".join(map(chr, [size]*size))

def xorstring(a, b):
    return "".join(map(lambda x: chr(ord(x[0]) ^ ord(x[1])) ,zip(a, b)))

key = 'YELLOW SUBMARINE'
mode = AES.MODE_CBC
IV = "\0"*16

encryptor = AES.new(key, mode, IV)
text = "alert('MZA who was that?');\n"
text = PKCSv7(text)
ciphertext = encryptor.encrypt(text)
orgindta = ciphertext.encode("hex")

print orgindta[-32:]

decryptor = AES.new(key, mode,IV)
target = decryptor.decrypt(ciphertext[-16:])

attack_prefix = "alert('Ayo, the Wu is back!'); //"

offset = 0x656565656565

while 1:
    offset += 1
    padding = '\t'*9
    attack = attack_prefix + ('%012x'%offset).decode("hex") + padding
    encryptor = AES.new(key, mode, IV)
    ciphertext = encryptor.encrypt(attack)
    constant = ciphertext[-16:]

    fixed = xorstring(target, constant)
    finalattack = attack + fixed

    encryptor = AES.new(key, mode, IV)
    ciphertext = encryptor.encrypt(finalattack)

    if ciphertext.encode("hex")[-32:] == orgindta[-32:]:
        print ciphertext.encode("hex")[-32:]
        with open("50.html",'w') as f:
            print >>f, "<html><body><script>"
            print >>f, finalattack
            print >>f, "</script></body></html>"
        break
