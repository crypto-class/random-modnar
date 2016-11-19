# coding=utf-8
from Crypto.Cipher import AES
import re
import random

def key_generate():#key生成
	global key
	key_space="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
	key=random.sample(key_space,16) 
	key=''.join(key)

def iv_generate():#iv生成
	iv_space="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
	iv=random.sample(iv_space,16) 
	iv=''.join(iv)	
	return iv
def padPKCS7(p, block_length):#填充
    pad = block_length - (len(p) % block_length)
    p=p+chr(pad)*pad
    print len(p)
    return p

def CBC_MAC(key, iv, p):#计算MAC
	cipher = AES.new(key, AES.MODE_CBC, iv)
	c = cipher.encrypt(padPKCS7(p, 16))
	return c[-16:]

def strxor(stringa,stringb):
	string=''
	for i in xrange(0,min(len(stringa),len(stringb))):
		string=string+chr(ord(stringa[i])^ord(stringb[i]))
	return string

def server_1(sended_message):#服务器
	message=sended_message[:-32]
	iv=sended_message[-32:-16]
	mac=sended_message[-16:]
	if(CBC_MAC(key, iv, message)==mac):
		print "verify right!"
		for x in message.split('&'):
			k,v=x.split('=')
			if(k=='from'):
				sender=v
			elif(k=='dest'):
				accepter=v
			else:
				account=v
		print sender+' pay '+account+'$ to '+accepter
	else:
		print 'masseage has been changed!'

def client_1(sender,recepter,account):#客户端
	global capture_message
	if not re.match('^[A-Za-z]+$', sender):
		print "sender error"
	elif not re.match('^[A-Za-z]+$', recepter):
		print "recepter error"
	elif not re.match('^[0-9]*\.[0-9]+|[0-9]+$',account):
		print "account error"
	else:
		#print "sending"
		message="from="+sender+"&dest="+recepter+"&account="+account
		iv=iv_generate()
	sended_message = message + iv + CBC_MAC(key, iv, message)
	server_1(sended_message)
	capture_message=sended_message

def attack_1():#攻击者
	sended_message=capture_message
	message=sended_message[:-32]
	iv=sended_message[-32:-16]
	mac=sended_message[-16:]
	iv=iv[0:5]+strxor(iv[5:11],strxor(message[5:11],'hacker'))+iv[11:]
	message=message[0:5]+'hacker'+message[11:]
	hacked_message=message+iv+mac
	server_1(hacked_message)

if __name__ == '__main__':
	key_generate()
	print key
	client_1("aaaaaa","bbbbbb","100")#客户端正常发送消息
	attack_1()#攻击者劫持消息（用全局变量模拟劫持信息），对消息中IV进行异或来达到修改原文的目的。