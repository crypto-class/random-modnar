#!/usr/bin/python

'''
File name: main.py
Created by: WRH
Email: o@seceve.com
http://www.cryptopals.com/sets/8/challenges/58
'''


# Implement Pollard's kangaroo algorithm. Here are some (less accommodating) group parameters:
p = 11470374874925275658116663507232161402086650258453896274534991676898999262641581519101074740642369848233294239851519212341844337347119899874391456329785623
q = 335062023296420808191071248367701059461
j = 34233586850807404623475048381328686211071196701374230492615844865929237417097514638999377942356150481334217896204702
g = 622952335333961296978159266084741085889881358738459939978290179936063635566740258555167783009058567397963466103140082647486611657350811560630587013183357

# And here's a sample y:
y = 7760073848032689505395005705677365876654629189298052775754597607446617558600394076764814236081991643094239886772481052254010323780165093955236429914607119



# This is a generic attack for computing a discrete logarithm (or "index") known to lie within a certain contiguous range [a, b]. It has a work factor approximately the square root of the size of the range.
# The index of y is in the range [0, 2^20]. Find it with the kangaroo algorithm.
a = 0
b = 2**20

# There are a couple implementation details we've glossed over - specifically the function f and the iteration count N. I do something like this:
def f(y,k):
    return pow(2,(y % k))

# The first is our control sequence. This is the tame kangaroo in Pollard's example. We do something like this:
def tame_kangaroo(b,g,p,N,k):
    xT = 0
    yT = pow(g,b,p)
    i = 1
    while i < N:
        xT = xT + f(yT,k)
        yT = (yT * pow(g,f(yT,k),p)) % p
        i += 1
    return xT,yT

# Now: let's catch that wild kangaroo. We'll do a similar loop, this time starting from y. Our hope is that at some point we'll collide with the tame kangaroo's path. If we do, we'll eventually end up at the same place. So on each iteration, we'll check if we're there.
def wild_kangaroo(y,xT,yT,a,b,g,p,k):
    xW = 0
    yW = y
    while xW < b - a + xT:
        xW = xW + f(yW,k)
        yW = (yW * pow(g,f(yW,k),p)) % p
        if yW == yT:
            return b + xT - xW






# For some k, which you can play around with. Making k bigger will allow you to take bigger leaps in each loop iteration.
for k in range(1,20):
    print 'k = ' + str(k),
    # N is then derived from f - take the mean of all possible outputs of f and multiply it by a small constant, e.g. 4. You can make the constant bigger to better your chances of finding a collision at the (obvious) cost of extra computation. The reason N needs to depend on f is that f governs the size of the jumps we can make. If the jumps are bigger, we need a bigger runway to land on, or else we risk leaping past it.
    N = ( f(0,k) + f(b,k)) / 2  * 2

    xT,yT = tame_kangaroo(b,g,p,N,k)
    A = wild_kangaroo(y,xT,yT,a,b,g,p,k)
    print ' | A = ' + str(A)

# 705485
A = 705485

print '%s ^ %s = %s' % (g,A,y)
print 
print pow(g,A,p)




