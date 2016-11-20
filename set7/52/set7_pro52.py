import hmac
import sys



def bad_hash_block(msg,iv):
	mac = str2bin(hmac.new(iv,msg).hexdigest())[:16]
	#print mac
	return mac

def str2bin(string):
	string_bin = ""
	for x in string:
		the_bin = bin(ord(x))[2:]
		if((8 - len(the_bin))>0):
			the_bin = '0'*(8 - len(the_bin)) + the_bin
		string_bin = string_bin + the_bin
	return string_bin

def padding(msg):
	over_pad = len(msg)%16
	msg = msg + chr(1) + chr(0)*((over_pad - 2)%16) + chr(len(msg))
	return msg

def Merkle_Damgard(msg,iv):# iv:16 Byte blank_length:16 Byte 
	m_block = []
	if(len(iv)<8):
		iv = '0'*(8 - len(iv)) + iv
	msg = padding(msg)
	for m in (m_block.append(msg[x:x + 16]) for x in xrange(0,len(msg), 16)):
		iv = bad_hash_block(m,iv)
	print iv
	return iv#mac

def find_a_collision(iv):
	H = {}
	collision_space1 = "abcdefghijklmnop"
	collision_space2 = "ABCDEFGHIJKLMNOP"
	for x in xrange(0,2**16):
		choice_base = bin(x)[2:]
		if(len(choice_base)<16):
			choice_base = '0'*(16 - len(choice_base)) + choice_base
		test_string = ''
		for index,x in enumerate(choice_base):
			if (x == '0'):
				test_string = test_string + collision_space1[index]
			else:
				test_string = test_string + collision_space2[index]
		h = bad_hash_block(test_string,iv)
		if(H.has_key(h)):
			#print (h,test_string,H[h])
			return (h,test_string,H[h])
		H[h] = test_string 
	print "cant find a collision"
	sys.exit(0)
		
def extend_collision(iv,collision):
	h,co1,co2 = find_a_collision(iv)
	return h, [x  +  co for x in collision for co in [co1, co2]]

def Merkle_Damgard_collision(iv,num):
	collision = []
	iv,co1,co2 = find_a_collision(iv)
	collision = [co1,co2]
	print "The first collision:"
	print collision
	for i in xrange(0,num):
		iv,collision = extend_collision(iv,collision)
		print "The " + str(i + 1) + " times extend:"
		print collision
		print len(collision)
	return collision

if __name__  ==    '__main__':
	iv = "0000000000000000"
	#Merkle_Damgard("aaaaaaa",iv)
	collision = Merkle_Damgard_collision(iv,3)
	for x in collision:
		Merkle_Damgard(x,iv)