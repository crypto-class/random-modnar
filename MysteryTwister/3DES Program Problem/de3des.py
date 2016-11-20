from Crypto.Cipher import DES3
import binascii
import string

file = open('./mtc3-shoellhammer-01-3des.txt')
ciphertext = file.read().replace(' ','').replace('\n','')
ciphertext = binascii.a2b_hex(ciphertext)
l_ciphertext = ciphertext[0:32]#only test for first line
key = 'COPACOBANA'

def cipherDecode3DES(key,ciphertext):
    cipher = DES3.new(key, DES3.MODE_CBC,'\x00'*8)
    message = cipher.decrypt(ciphertext)
    return message

def burp(ciphertext):
    all_digits = []
    for digits in xrange(0,999999):
        digits = str(digits)
        if len(digits)<6:#padding for 6 bytes
            digits = (6-len(digits))*'0'+digits
        ascii_key = key + digits
        message = cipherDecode3DES(ascii_key,ciphertext)

        if all(c in string.printable for c in message):
            print 'The first line of the plaintext is :\n',message
            print '**********************'
            print 'The digits is :',digits
            all_digits.append(digits)                               
    return all_digits
            #break


digits = burp(l_ciphertext)
for i in digits:
    passkey = key + i
    message = cipherDecode3DES(passkey,ciphertext)
    print '*************************'
    print 'The key is :',passkey
    print '*************************'
    print 'The length of message is :',len(message)
    print '*************************'
    print 'The all message is :', message


