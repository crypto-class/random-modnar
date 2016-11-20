from Crypto.Cipher import AES
from binascii import a2b_hex
from decimal import Decimal as D
from decimal import getcontext

c_s=0xfd0b934c23288975648cd1d03ed3c5e2
c_p=0xc0eacf32dc0492464d9616fefc3d01f56589a137781bf6cf56784dea1c44ef52d61b1025655f370eb78646716f93e0a5
n=0x9c5f36caf9adc60b4447897c639f1564ed0709251147276de030db395555c8afed912a198b334bd230198173128298126e958e38cac653e061035e300505eed1
e=0x3

getcontext().prec = 1000 #change

m = int(pow(D(c_p),D(1)/D(3)))+1 #add 1 to 
m = a2b_hex(hex(m)[2:-1]) #change hex to str

c_s = a2b_hex(hex(c_s)[2:-1]) #change hex to str
cipher = AES.new(m, AES.MODE_ECB)
print cipher.decrypt(c_s) #Get the answer
#Diffie Rocks !!!