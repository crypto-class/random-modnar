from rc4 import *
import random
import zlib
import base64
import string

global key
def key_generate(len_key):
	key_space=alphabet=string.ascii_letters + string.digits + '+/='
	key=''.join(random.sample(key_space,len_key))
	return key

def make_frame(p):
	frame="POST / HTTP/1.1"+"Host: hapless.com"+"Cookie: sessionid=TmV2ZXIgcmV2ZWFsIHRoZSBXdS1UYW5nIFNlY3JldCE=Content-Length: ("+str(len(p))+")"+"Content:"+p
	return frame
def compress_rc4(P):
    frame=make_frame(P)
    #print frame
    compressed_frame =zlib.compress(frame.encode('ascii'))
    cipher=en_rc4(compressed_frame,key)
    #print cipher
    return len(cipher)

def Nextchr(knownStr):
    ch_min = ''
    ch_sz = 0
    alphabet = string.ascii_letters + string.digits + '+/='
    #padding = getPadding(oracle, ('sessionid=' + knownStr + '~') * 8)
    for i in range(len(alphabet)):
        ch = alphabet[i]
        s = 'sessionid=' + knownStr + ch
        sz = compress_rc4(s * 8)
        if ch_min == '' or sz < ch_sz:
            ch_min = ch
            ch_sz = sz
    return ch_min


def recover_sessionid():
    knownStr = ''
    for i in range(0, 44):
        knownStr += Nextchr(knownStr)
        print "try_num:"+str(i)
    	print "sessionid:"+knownStr
    return knownStr

if __name__ == '__main__':
	key=key_generate(16)
	session=recover_sessionid()
	if(session=="TmV2ZXIgcmV2ZWFsIHRoZSBXdS1UYW5nIFNlY3JldCE="):
		print 'success'
		print "sessionid is "+ session
	else:
		print 'error'