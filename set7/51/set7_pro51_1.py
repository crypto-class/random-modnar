from Crypto.Cipher import AES
import string
import zlib
import random

global key
global iv

def key_generate(len_key):
	key_space=alphabet=string.ascii_letters + string.digits + '+/='
	key=''.join(random.sample(key_space,len_key))
	return key

def iv_generate(len_iv):
	iv_space=alphabet=string.ascii_letters + string.digits + '+/='
	iv=''.join(random.sample(iv_space,len_iv))
	return iv

def padPKCS7(p, block_length):
    pad=block_length - (len(p) % block_length)
    p=p+chr(pad)*pad
    #print len(p)
    return p

def make_frame(p):
	frame="POST / HTTP/1.1"+"Host: hapless.com"+"Cookie: sessionid=TmV2ZXIgcmV2ZWFsIHRoZSBXdS1UYW5nIFNlY3JldCE=Content-Length: ("+str(len(p))+")"+"Content:"+p
	return frame

def compress_cbc(P):
    frame=make_frame(P)
    compressed_frame =zlib.compress(frame.encode('ascii'))
    cipher=AES.new(key, AES.MODE_CBC, iv)
    encrypted_frame=cipher.encrypt(padPKCS7(compressed_frame, 16))
    return len(encrypted_frame)

def Pad_MAX(s):
    l =compress_cbc(s)
    padding_sapce='!@#$%^&*(){}<>?:;'
    padding=''
    for i in range(len(padding_sapce)):
        padding += padding_sapce[i]
        l_1=compress_cbc(padding + s)
        if(l_1 > l):
            return padding

def NextChr(front_session):
    real_ch=''
    min_size=0
    alphabet = string.ascii_letters + string.digits + '+/='
    padding=Pad_MAX(('sessionid=' + front_session+'!') * 16)
    for i in range(len(alphabet)):
        ch=alphabet[i]
        s='sessionid=' + front_session + ch

        size=compress_cbc(padding + s * 16)
        if real_ch == '' or size < min_size:
            real_ch=ch
            min_size=size
    return real_ch

def recover_session():
    front_session=''
    for i in range(0, 44):
        front_session += NextChr(front_session)
        print "try"+str(i)
        print "sessionid:"+front_session
    return front_session

if __name__ == '__main__':
    key=key_generate(16)
    iv=iv_generate(16)
    print recover_session()