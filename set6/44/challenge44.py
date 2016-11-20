from binascii import a2b_hex
from collections import Counter
from hashlib import sha1
from gmpy2 import invert,mpz


file = open('44.txt')

q = 0xf4f47f05794b256174bba6e9b396a7707e563c5b
y = 0x2d026f4bf30195ede3a088da85e398ef869611d0f68f0713d51c9c1a3a26c95105d915e2d8cdf26d056b86b8a7b85519b1c23cc3ecdc6062650462e3063bd179c2a6581519f674a61f1d89a1fff27171ebc1b93d4dc57bceb7ae2430f98a6a4d83d8279ee65d71c1203d2c96d65ebbf7cce9d32971c3de5084cce04a2e147821
msg_hash = 0xd2d0714f014a9784047eaeccf956520045c45265

#To get the list of m and s
m = []
s = []
r = []
for eachline in file:
	if eachline[0:2]=='m:':
		m.append(eachline[3:-1])

	elif eachline[0:2]=='s:':
		s.append(eachline[3:-1])
	elif eachline[0:2]=='r:':
		r.append(eachline[3:-1])

for x in xrange(len(m)):
	m[x] = int(m[x],16)

for x in xrange(len(s)):
	s[x] = int(s[x])


# It should not be hard to find the messages for which we have accidentally used a repeated "k". Given a pair of such messages, you can discover the "k" we used with the following formula:
repeated_key_r =  Counter(r).most_common(3)[2][0]

index = []
for x in xrange(len(r)):
	if r[x]==repeated_key_r:
		index.append(x)
#Get the message and sign which use the same k
m1 = m[index[0]];	m2 = m[index[1]];
s1 = s[index[0]];	s2 = s[index[1]];

print '------'


#     (m1 - m2)
# k = --------- mod q
#     (s1 - s2)

inv_s = invert((s1-s2),q)
k =  (inv_s * (m1-m2)) % q

repeated_key_r = mpz(repeated_key_r)
inv_r = invert(repeated_key_r,q)

def H(message):
     return sha1(message).hexdigest()

def recover_x(s,m,r):
	return (r *(s*k - m))%q

x1 = recover_x(s1,m1,inv_r)
x2 = recover_x(s2,m2,inv_r)

print 'Had recover the key:',x1==x2
hex_sha = 'ca8f6f7c66fa362d40760d135b763eb8527d3d52'
print 'The result of verify the key:',H('%x'%x1)==hex_sha
# ca8f6f7c66fa362d40760d135b763eb8527d3d52
