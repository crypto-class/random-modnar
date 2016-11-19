from challenge21 import MT19937

#There are two methods to solve this challenge.

#https://jazzy.id.au/2010/09/22/cracking_random_number_generators_part_3.html
def untemper(value):
	assert(value < 2**32)
	assert(value >= 0)

	y = value

	y ^= y>>18
	y = unLeftShift(y, 15, 0xefc60000)
	y = unLeftShift(y, 7, 0x9d2c5680)
	# Inverse of y = y ^ (y >> 11)
	top = y & 0xffe00000
	mid = y & 0x001ffc00
	low = y & 0x000003ff

	y = top | ((top >> 11) ^ mid) | ((((top >> 11) ^ mid) >> 11) ^ low)
	return y

def unLeftShift(value,shift,mask):
	i = 0
	result = 0
	while i*shift < 32:
		part = (0xffffffff >> (32 - shift)) << (shift * i)
		part &= value
		value ^= (part << shift) & mask
		result |= part
		i += 1
	return result

def untemper1(value):
    assert(value < 2**32)
    assert(value >= 0)

    y = value

    # Inverse of y = y ^ (y >> 18)
    y = y ^ (y >> 18)

    # Inverse of y = y ^ ((y << 15) & 0xefc60000)
    # hex(0xefc60000>>15) #0x1df8c
    y = y ^ ((y & 0x1df8c) << 15)

    # Inverse of y = y ^ ((y << 7) & 0x9d2c5680)
    t = y
    t = ((t & 0x0000002d) << 7) ^ y
    t = ((t & 0x000018ad) << 7) ^ y
    t = ((t & 0x001a58ad) << 7) ^ y
    y = ((t & 0x013a58ad) << 7) ^ y

    # Inverse of y = y ^ (y >> 11)
    top = y & 0xffe00000
    mid = y & 0x001ffc00
    low = y & 0x000003ff

    y = top | ((top >> 11) ^ mid) | ((((top >> 11) ^ mid) >> 11) ^ low)

    return y

def temper(y):
	y ^= y>>11
	y ^= (y<<7) & 2636928640 #0x9d2c5680
	y ^= (y<<15) & 4022730752 #0xefc60000
	y ^= y>>18
	return y

def reTemperTest():
	"""
	1	0b111000111101110101
	2	0b111000111100000100
		0b1110001111000001000000000		           #(2)<<7
 0b10011101001011000101011010000000 		       #0x9d2c5680
 		0b1000001000000001000000000   			   #(y<<7) & 2636928640
	3	0b1000001111000110100000100 			   
0b1000001111000110100000100000000000000000         #(3)<<15
		0b11101111110001100000000000000000         #0xefc60000
	&   0b11000110100000100000000000000000         #(y<<15) & 4022730752
	4	0b11000111100001011000110100000100
  	xor 0b00000000000000000011000111100001		   #(4)>>18
	5	0b11000111100001011011110011100101
	"""
	num = 233333
	print "The result of untemper :",untemper(temper(num))==num
	print "The result of untemper1 :",untemper1(temper(num))==num


def unMT19937(value):
	unMT19937 = MT19937(0)
	for x in xrange(len(unMT19937.MT)):
		unMT19937.MT[x] = untemper(value.extract_number())
	return unMT19937

if __name__ == "__main__":
	reTemperTest()
	rnd = MT19937(23333)
	unrnd = unMT19937(rnd)
	for x in xrange(10):
		if unrnd.extract_number() != rnd.extract_number():
			print "Failed"
	print "Completed"