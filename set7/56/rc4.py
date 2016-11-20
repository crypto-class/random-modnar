# #coding=utf-8

# import sys,os,hashlib,time,base64
# class rc4:

#   def __init__(self,public_key = None):
#     self.public_key = public_key or 'none_public_key'
#     self.public_key = hashlib.md5(public_key).hexdigest()
#   def encode(self,string):
#     self.result = ''
#     self.docrypt(string)
#     self.result = base64.b32encode(self.result)
#     return self.result

#   def decode(self,string):
#     self.result = ''
#     string = base64.b32decode(string)
#     self.docrypt(string)
#     return self.result

#   def docrypt(self,string):
#     string_lenth = len(string)
#     result = ''
#     box = list(range(256))
#     randkey = []
#     key_lenth = len(self.public_key)

#     for i in xrange(255):
#       randkey.append(ord(self.public_key[i % key_lenth]))

#     for i in xrange(255):
#       j = 0
#       j = (j + box[i] + randkey[i]) % 256
#       tmp = box[i]
#       box[i] = box[j]
#       box[j] = tmp

#     for i in xrange(string_lenth):
#       a = j = 0
#       a = (a + 1) % 256
#       j = (j + box[a]) % 256
#       tmp = box[a]
#       box[a] = box[j]
#       box[j] = tmp
#       self.result += chr(ord(string[i]) ^ (box[(box[a] + box[j]) % 256]))

class rc4:
    def __init__(self, key):
        key = map(ord, key)
        self.S = [x for x in range(256)]
        j = 0
        for i in range(256):
            j = (j + self.S[i] + key[i % len(key)]) & 0xff
            self.swap(i, j)
        self.i = 0
        self.j = 0

    def swap(self, i, j):
            self.S[i], self.S[j] = self.S[j], self.S[i]

    def encrypt(self, pt):
        ct = []

        for p in map(ord, pt):
            self.i = (self.i + 1) & 0xff
            self.j = (self.j + self.S[self.i]) & 0xff
            self.swap(self.i, self.j)
            k = self.S[(self.S[self.i] + self.S[self.j]) & 0xff]
            ct.append(p ^ k)

        return ''.join(chr(x) for x in ct)

    def decrypt(self, ct):
        return self.encrypt(ct)

def run_tests():
    def test(key, plaintext, ciphertext):
        ciphertext = ''.join(chr(x) for x in ciphertext)
        assert ciphertext == rc4(key).encrypt(plaintext)
        assert plaintext == rc4(key).decrypt(ciphertext)

    test('Key', 'Plaintext', [0xbb, 0xf3, 0x16, 0xe8, 0xd9, 0x40, 0xaf, 0x0a, 0xd3])
    test('Wiki', 'pedia', [0x10, 0x21, 0xbf, 0x04, 0x20])
    test('Secret', 'Attack at dawn', [0x45, 0xa0, 0x1f, 0x64, 0x5f, 0xc3, 0x5b, 0x38, 0x35, 0x52, 0x54, 0x4b, 0x9b, 0xf5])

if __name__ == '__main__':
    run_tests()