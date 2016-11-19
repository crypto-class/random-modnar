from Crypto.Cipher import AES

def CTRDecrypt(key, iv, data):
    cipher = AES.new(key, AES.MODE_CTR, counter=lambda: iv)
    return cipher.decrypt(data)


ciphertext = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==".decode('base64')

key = "YELLOW SUBMARINE"
iv1 = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
iv2 = "\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00"
iv3 = "\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00"
iv4 = "\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00"

ct1 = ciphertext[0:16]
ct2 = ciphertext[16:32]
ct3 = ciphertext[32:48]
ct4 = ciphertext[48:64]

msg =  CTRDecrypt(key, iv1, ct1) + CTRDecrypt(key, iv2, ct2) + CTRDecrypt(key, iv3, ct3) + CTRDecrypt(key, iv4, ct4)
print msg