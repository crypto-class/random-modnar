class MT19937(object):
	"""docstring for randomCreate"""
	def __init__(self,seed):
		self.MT = list(xrange(624))
		self.index = 0
		self.initialize_generator(seed)

 # function initialize_generator(int seed) {
 #     i := 0
 #     MT[0] := seed
 #     for i from 1 to 623 { 
 #         MT[i] := last 32 bits of(1812433253 * (MT[i-1] xor (right shift by 30 bits(MT[i-1]))) + i) // 0x6c078965
 #     }
 # }
	def initialize_generator(self,seed):
		# self.index = 0
		self.MT[0] = seed
		for x in xrange(1,len(self.MT)):
			self.MT[x] = (1812433253 * (self.MT[x-1] ^ (self.MT[x-1])>>30) + x ) & 0xffffffff
 
 # // Extract a tempered pseudorandom number based on the index-th value,
 # // calling generate_numbers() every 624 numbers
 # function extract_number() {
 #     if index == 0 {
 #         generate_numbers()
 #     }
 
 #     int y := MT[index]
 #     y := y xor (right shift by 11 bits(y))
 #     y := y xor (left shift by 7 bits(y) and (2636928640)) // 0x9d2c5680
 #     y := y xor (left shift by 15 bits(y) and (4022730752)) // 0xefc60000
 #     y := y xor (right shift by 18 bits(y))

 #     index := (index + 1) mod 624
 #     return y
 # }
	def extract_number(self):
		if self.index == 0:
			self.generate_numbers()
		y = self.MT[self.index]
		y ^= y>>11
		y ^= (y<<7) & 2636928640 #0x9d2c5680
		y ^= (y<<15) & 4022730752 #0xefc60000
		y ^= y>>18

		self.index = (self.index+1)%624
		return y

 # // Generate an array of 624 untempered numbers
 # function generate_numbers() {
 #     for i from 0 to 623 {
 #         int y := (MT[i] & 0x80000000)                       // bit 31 (32nd bit) of MT[i]
 #                        + (MT[(i+1) mod 624] & 0x7fffffff)   // bits 0-30 (first 31 bits) of MT[...]
 #         MT[i] := MT[(i + 397) mod 624] xor (right shift by 1 bit(y))
 #         if (y mod 2) != 0 { // y is odd
 #             MT[i] := MT[i] xor (2567483615) // 0x9908b0df
 #         }
 #     }
 # }
	def generate_numbers(self):
		for i in xrange(len(self.MT)):
			y = (self.MT[i] & 0x80000000) +  (self.MT[(i+1) % 624] & 0x7fffffff) #bit 31 (32nd bit) of MT[i]
						#bits 0-30 (first 31 bits) of MT[...]
			self.MT[i] = self.MT[ (i+397) % 624 ] ^ y>>1
			if (y%2)!=0: #y is odd
				self.MT[i] = self.MT[i] ^ 2567483615 #0x9908b0df


def main():
	# #Test codes
	rnd = MT19937(1) #please input the seed


	#Cause this is the fake random numbers generator, it will generate the same numbers when the index is same.
	print rnd.extract_number()
	print rnd.extract_number() #when the index number is change, the output number will also change
	print rnd.extract_number()
	print rnd.extract_number()
	print rnd.extract_number()

if __name__ == '__main__':
	main()
