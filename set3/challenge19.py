import binascii
from Crypto.Cipher import AES
from Crypto import Random
from string import ascii_letters
import itertools
from itertools import cycle

def CTREncrypt(data): #CTR mode encrypt function
    cipher = AES.new(rndkey, AES.MODE_CTR, counter=lambda: iv)
    return cipher.encrypt(data)

def getletterKeyChar(ciphertext, i):
	for j in xrange(256):
		decrypted = [ord(x[i]) ^ j for x in ciphertext]
		if all([chr(x) in letter for x in decrypted]):
			yield j

def strxor(text, key):
    dec = [chr(ord(i) ^ ord(j)) for i,j in zip(text,cycle(key))]
    return "".join(dec)

def extendKey(key, ciphertext, guess):
	key = map(ord,key)
	key = key + list(ord(guess[i]) ^ ord(ciphertext[len(key)+i]) for i in xrange(len(guess)) )
	key = map(chr,key)
	return key


strings = ["SSBoYXZlIG1ldCB0aGVtIGF0IGNsb3NlIG9mIGRheQ==",
			"Q29taW5nIHdpdGggdml2aWQgZmFjZXM=",
			"RnJvbSBjb3VudGVyIG9yIGRlc2sgYW1vbmcgZ3JleQ==",
			"RWlnaHRlZW50aC1jZW50dXJ5IGhvdXNlcy4=",
			"SSBoYXZlIHBhc3NlZCB3aXRoIGEgbm9kIG9mIHRoZSBoZWFk",
			"T3IgcG9saXRlIG1lYW5pbmdsZXNzIHdvcmRzLA==",
			"T3IgaGF2ZSBsaW5nZXJlZCBhd2hpbGUgYW5kIHNhaWQ=",
			"UG9saXRlIG1lYW5pbmdsZXNzIHdvcmRzLA==",
			"QW5kIHRob3VnaHQgYmVmb3JlIEkgaGFkIGRvbmU=",
			"T2YgYSBtb2NraW5nIHRhbGUgb3IgYSBnaWJl",
			"VG8gcGxlYXNlIGEgY29tcGFuaW9u",
			"QXJvdW5kIHRoZSBmaXJlIGF0IHRoZSBjbHViLA==",
			"QmVpbmcgY2VydGFpbiB0aGF0IHRoZXkgYW5kIEk=",
			"QnV0IGxpdmVkIHdoZXJlIG1vdGxleSBpcyB3b3JuOg==",
			"QWxsIGNoYW5nZWQsIGNoYW5nZWQgdXR0ZXJseTo=",
			"QSB0ZXJyaWJsZSBiZWF1dHkgaXMgYm9ybi4=",
			"VGhhdCB3b21hbidzIGRheXMgd2VyZSBzcGVudA==",
			"SW4gaWdub3JhbnQgZ29vZCB3aWxsLA==",
			"SGVyIG5pZ2h0cyBpbiBhcmd1bWVudA==",
			"VW50aWwgaGVyIHZvaWNlIGdyZXcgc2hyaWxsLg==",
			"V2hhdCB2b2ljZSBtb3JlIHN3ZWV0IHRoYW4gaGVycw==",
			"V2hlbiB5b3VuZyBhbmQgYmVhdXRpZnVsLA==",
			"U2hlIHJvZGUgdG8gaGFycmllcnM/",
			"VGhpcyBtYW4gaGFkIGtlcHQgYSBzY2hvb2w=",
			"QW5kIHJvZGUgb3VyIHdpbmdlZCBob3JzZS4=",
			"VGhpcyBvdGhlciBoaXMgaGVscGVyIGFuZCBmcmllbmQ=",
			"V2FzIGNvbWluZyBpbnRvIGhpcyBmb3JjZTs=",
			"SGUgbWlnaHQgaGF2ZSB3b24gZmFtZSBpbiB0aGUgZW5kLA==",
			"U28gc2Vuc2l0aXZlIGhpcyBuYXR1cmUgc2VlbWVkLA==",
			"U28gZGFyaW5nIGFuZCBzd2VldCBoaXMgdGhvdWdodC4=",
			"VGhpcyBvdGhlciBtYW4gSSBoYWQgZHJlYW1lZA==",
			"QSBkcnVua2VuLCB2YWluLWdsb3Jpb3VzIGxvdXQu",
			"SGUgaGFkIGRvbmUgbW9zdCBiaXR0ZXIgd3Jvbmc=",
			"VG8gc29tZSB3aG8gYXJlIG5lYXIgbXkgaGVhcnQs",
			"WWV0IEkgbnVtYmVyIGhpbSBpbiB0aGUgc29uZzs=",
			"SGUsIHRvbywgaGFzIHJlc2lnbmVkIGhpcyBwYXJ0",
			"SW4gdGhlIGNhc3VhbCBjb21lZHk7",
			"SGUsIHRvbywgaGFzIGJlZW4gY2hhbmdlZCBpbiBoaXMgdHVybiw=",
			"VHJhbnNmb3JtZWQgdXR0ZXJseTo=",
			"QSB0ZXJyaWJsZSBiZWF1dHkgaXMgYm9ybi4="]

letter = ascii_letters + ' ' + ',' + '.'

strings = map(binascii.a2b_base64,strings) #base64 decode
iv = '\x00'*16 #set nonce value to 0
rndkey = Random.new().read(AES.block_size) #generate random AES key
strings = map(CTREncrypt,strings) #encrypt each line


def main():
	msg = [getletterKeyChar(strings,x) for x in xrange(10)]
	k = list(itertools.islice(itertools.product(*msg), 1))[0]

	k = map(chr,k)
	print 'First try:'
	print strxor(strings[1],k)[0:10]#coming wit
	print '='*32

	k = extendKey(k, strings[1], 'h') #propose the incomplete part should be character 'h'

	print 'Second times try:'
	print strxor(strings[3],k)[0:11]#Eighteenth-
	print '='*32

	k = extendKey(k, strings[3], 'century') #propose the incomplete part should be words 'century'
	k = k[:-2]  #key length should be 16,the pre k length is 18
	print 'The decrypt message:\n'
	for x in strings:
		print strxor(x,k) #sometimes it is correct but the upper or lower probably incorrect
	#It is base on frequency Please do one more try ;)


if __name__ == '__main__':
	main()
			
