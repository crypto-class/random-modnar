import binascii
from Crypto.Cipher import AES
from Crypto import Random
from string import ascii_letters
from collections import Counter
from itertools import cycle


#read file
file = open("../files/20.txt")
strings = []
for each_line in file:
	strings.append(each_line)

strings = map(binascii.a2b_base64,strings) #base64 decode
verify = strings
iv = '\x00'*16 #set nonce value to 0
rndkey = Random.new().read(AES.block_size) #generate random AES key
letter = ascii_letters + ' ' + ',' + '.' #as message to guess key

def CTREncrypt(data): #CTR mode encrypt function
    cipher = AES.new(rndkey, AES.MODE_CTR, counter=lambda: iv)
    return cipher.encrypt(data)

strings = map(CTREncrypt,strings) #encrypt each line


def guessKey(ciphertext,i):
	a = []
	for x in ciphertext:
		for j in letter:
			a.append(ord(x[i])^ord(j))
	return a

def strxor(text, key):
    dec = [chr(ord(i) ^ ord(j)) for i,j in zip(text,cycle(key))]
    return "".join(dec)


def main():
	#group by AES block_size to get the sample
	a = []
	for x in strings:
		a.append(x[:16])
		a.append(x[16:32]) 
		# a.append(x[32:48]) #add the percision 
		# a.append(x[48:64])

	#Get the key
	keys = []
	for x in xrange(16):
		string = guessKey(a,x)
		keys.append(Counter(string).most_common(1)[0][0]) #to Get the most possible ones
	keys = ''.join(map(chr,keys))


	msg = ''
	for x in strings:
		msg += strxor(x,keys)

	print msg
	print msg==''.join(verify) #nearly 100% equal

if __name__ == '__main__':
	main()





