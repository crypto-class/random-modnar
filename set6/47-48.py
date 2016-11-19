#!/usr/bin/python

import binascii as bin

def set47(z):
    y = (z * m0) % n
    rr = bin.hexlify(bin.unhexlify('%0256x' % y))
    #print rr[0:4]
    if (rr[0:4]) == "0002":
        if "00" in rr[4:]:
            return True
        else: 
            return False

    else:
        return False

def ceil(x,y):
    return x/y + (x%y != 0)

def floor(x,y):
    return x//y

n = 0x00e133440e6948ba3905e722442723bc5741fef35213731f3871de50c904676fefc836dc9253ead0a79dc89c42647d1614a3e167550e9890c379045e8822451fc83bf53ed66885654e9658bff02d5cdb13d5e28e9ac650c380552195ee5927eb51f9e6fbeef8fde1639f05b15e677958042d922e2f62c024071839e7602ddfed6f
B = 2**1008
m0 = 0x00021111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111006B69636B2069742C20434375

s1 = ceil(n,3*B) # this is the starting value for s1

print "1.Starting search for s1 (from value %i)" % s1
i2a = 1          # counter for iterations
k = 128 # number of bytes of modulus 128 = 1024 bits
fk = '%0'+str(k*2)+'x' # format string to print hex 0 padded
while True:
    if set47(s1):   # call the (simulated) oracle
        break    # padding is correct, we have found s1
    i2a += 1
    s1 += 1      

print "2.Search done in %i iterations" % i2a
print "s1: %i" % s1

B2,B3 = 2*B,3*B # constants to avoid recomputing them any time
newM = set([])  # collects new intervals

a = B2
b = B3 - 1
si = s1

print "iterations :"
print ceil((a*si - B3 + 1),n),floor(((b)*si - B2),n) + 1
for r in range(ceil((a*si - B3 + 1),n),floor(((b)*si - B2),n) + 1):      
    aa = ceil(B2 + r*n,si)
    bb = floor(B3 - 1 + r*n, si)           
    newa = max(a,aa)
    newb = min(b,bb)                             
    if newa <= newb:                                
        newM |= set([ (newa, newb) ])
print "len" + str(len(newM))

while True:
    if len(newM) == 1:
        loner = newM.pop()
        newM.add(loner)
        a = loner[0]
        b = loner[1]
        r = ceil((b*si - B2)*2,n) # starting value for r
        i2c,nr = 0,1
        found = False
        while not found:
            for si in range(ceil((B2 + r * n),b),floor((B3-1 + r * n),a)+1):
                i2c += 1
                if set47(si):
                    found = True
                    break # padding is correct, we have found s1
            if not found:
                r  += 1   # try next value for r
                nr += 1
        #print "Search done in %i iterations" % (i2c)
        #print "explored values of r:  %i" % nr
        print "s_%i : %i" % (i2c,si)
    elif len(newM) > 1:
        si += 1
        while True:
            if set47(si):
                break    
            si += 1
        print "si: %i" % si
    newMM = set([])

    for (a,b) in newM: # for all intervals
        for r in range(ceil((a*si - B3 + 1),n),floor((b*si - B2),n) + 1):
            aa = ceil(B2 + r*n,si)
            bb = floor(B3 - 1 + r*n, si)
            newa = max(a,aa)
            newb = min(b,bb)
            if newa <= newb:
                newMM |= set([ (newa, newb) ])

    if len(newMM)>0:
        newM = newMM

    if len(newM) == 1:
        popped = newM.pop()
        if popped[0] == popped[1]:
            result = fk % popped[0]
            print "plaintext has been found : "+result
            break
        newM.add(popped)

str = bin.unhexlify(result.split('00')[2])
print str


