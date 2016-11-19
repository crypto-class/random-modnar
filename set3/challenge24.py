from challenge21 import MT19937
from Crypto.Random import random
from itertools import cycle
import struct
from string import ascii_letters,digits
import time


def strxor(text, key):
    dec = [chr(ord(i) ^ ord(j)) for i,j in zip(text,cycle(key))]
    return "".join(dec)

class MT19937Cipher(object):
	"""docstring for MT19937Cipher"""
	def __init__(self, seed):
		self.mt = MT19937(seed & 0xffff) #16-bit seed
		self.key = ''
	def encrypt(self,plaintext):
		assert(len(plaintext) > 0)
		keystream = self.key
		ciphertext = ''


		#Call outputs a keystream
		for x in xrange(len(plaintext)):
			keyblock = struct.pack('<L',self.mt.extract_number() & 0xff) #8-bit outputs
			text = struct.pack('<c',plaintext[x]) #each plaintext
			ciphertext += text 
			keystream += keyblock
		self.key = keystream

		return strxor(ciphertext,keystream) #XOR each byte of plaintext

	def decrypt(self,ciphertext):
		return strxor(ciphertext, self.key)
		# return self.encrypt(ciphertext)


key = random.getrandbits(16)

def verifyMT():
	mt = MT19937Cipher(key)
	msg = 'test'
	print 'The result of verify:',mt.decrypt(mt.encrypt(msg))==msg


def generatePrefix(prefixtype): 
	numbers = random.randrange(1,30)
	prefix = ''
	for x in xrange(numbers):
		prefix += prefixtype[random.randrange(0,len(prefixtype))]
	return prefix

def encryptPrefixedPlaintext(plaintext):
	prefix = generatePrefix(ascii_letters)
	prefix = ''
	cipher = MT19937Cipher(key)
	return cipher.encrypt(prefix+plaintext)

def recoverKey(msg):
	ciphertext = encryptPrefixedPlaintext(msg)
	for x in xrange(0xffff):
		mt = MT19937Cipher(x)
		test = mt.encrypt('A'*len(ciphertext))
		if ciphertext[-14:]==test[-14:]:
			print "We get the key:",x
			return x
	print 'Sorry, we lost'
		
def tokenGenerate():
	seed = int(time.time())
	mt = MT19937(seed)
	rndpassword = ''
	for x in xrange(10):
		rndpassword += str(mt.extract_number()&0xffff)
	return rndpassword

def checkTokenIsCurrentTime(token):
	seed = int(time.time())
	for x in xrange(-1000 ,1000): #time delay range
		rndpassword = ''
		mt = MT19937(seed+x)
		for i in xrange(10):
			rndpassword += str(mt.extract_number()&0xffff)
		if rndpassword==token:
			return True
	return False

if __name__=='__main__':
	verifyMT()
	msg = 'A'*14
	print "Start to recover the key"
	print '-'*30
	result = recoverKey(msg)
	print "The result of recover the key:",key==result
	print "Start to check Token"
	print '-'*30
	print "The result of check token:",checkTokenIsCurrentTime(tokenGenerate())
