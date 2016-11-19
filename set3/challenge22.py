from challenge21 import MT19937
from time import time
from random import randrange

#Just a function to test
def test():
    mt1 = MT19937(2333);
    mt2 = MT19937(2333);
    for i in range(10000):
        if (mt1.extract_number() != mt2.extract_number()):
            print("Failed!");

test()

delay1 = randrange(40,1000)
delay2 = randrange(40,1000)
timestamp = int(time())

#We don't need to wait the random seconds in real world.
#Just add current timestamp and delay ,and it will work to save time. ;)
def randomWait():
	mt = MT19937(timestamp + delay1) #The first time to wait a random numbers of seconds
	return mt.extract_number()

#Try to recover the timestamp by sub the numbers
def recoverTimestamp(rand,currentTime):
	#Suppose we had known the delay times lower than 2000s
	for x in xrange(2000): 
		mt = MT19937(currentTime-x)
		fakerand = mt.extract_number()
		if fakerand==rand:
			seed = currentTime - delay1 - delay2
			print "We get the seed:",seed
			return seed


if __name__ == "__main__":
	rand = randomWait()

	seed = recoverTimestamp(rand, int(time())+delay1+delay2)

	print "Check the seed:",seed==timestamp