from Crypto.Cipher import AES
from random import randrange
from Crypto import Random

# PKCS#7 padding
BS = AES.block_size
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS)
def pkcs7unpad(raw):
  pad = ord(raw[-1])
  if raw[-pad:] != chr(pad) * pad:
    return False
  return raw[:-pad]


class AESCipher(object):
	"""docstring for AESCipher"""
	def __init__(self):
		self.rndkey = Random.new().read(AES.block_size)	#generate random AES key 
		self.strings = [b"MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=",
						b"MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic=",
						b"MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw==",
						b"MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg==",
						b"MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl",
						b"MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA==",
						b"MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw==",
						b"MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8=",
						b"MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g=",
						b"MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93"]

	#first function
	def GetRandomStr(self):
		rndnum = randrange(0,len(self.strings)) #generate random number 
		padmsg = pad(self.strings[rndnum]) #padding the msg

		iv = Random.new().read(AES.block_size)
		cipher = AES.new(self.rndkey, AES.MODE_CBC, iv)
		msg = iv + cipher.encrypt(padmsg)
		return msg,iv

	def DecryptCheckPadding(self,msg,iv):
		msg = msg[16::]
		cipher = AES.new(self.rndkey, AES.MODE_CBC, iv)
		msg = cipher.decrypt(msg)
		if pkcs7unpad(msg):
			return True
		else :
			return False

def main():
	AEStest = AESCipher()
	ciphertext,iv = AEStest.GetRandomStr()
	print AEStest.DecryptCheckPadding(ciphertext,iv) #valid padding example
	print AEStest.DecryptCheckPadding(ciphertext+'\x00'*16,iv) #invalid padding example

if __name__ == '__main__':
	main()


