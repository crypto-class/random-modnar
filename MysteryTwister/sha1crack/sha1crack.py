from Crypto.Hash import SHA
import sys
import binascii
import itertools

sha = '67ae1a64661ac8b4494666f58c4822408dd0a3e4'

word = 'qwinQWIN'
l_word = list(itertools.combinations(word,4))

digits = '580%(='
l_digits = list(itertools.combinations(digits,3))

def sha_enc(message):
    h = SHA.new()
    h.update(message)
    return h.hexdigest()

def burp(strings):
    strings = ''.join(strings) #from list to string
    sha_crack = sha_enc(strings)
    if sha_crack == sha:
        print 'Good job\nGot it!!\nThe message is:',strings
        sys.exit(0)
        
def clean(check,word):
    for x in check:
        for i in xrange(len(word)/2):
            if set((word[i], word[i+len(word)/2])).issubset(x): #
                if x in check:
                    check.remove(x)

clean(l_word,word)
clean(l_digits,digits)

print 'Haha~ start to cracking!'
for x in l_word:
    for i in l_digits:
        words = ''.join(x+i)+'*'
        message = itertools.permutations(words)
        map(burp,message)
