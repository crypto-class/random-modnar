# coding=utf-8
from Crypto.Cipher import AES#攻击方式二
import re
import random


global sended_message_list
global key
global iv

sended_message_list=[]
iv=chr(0)*16

def key_generate():#key生成
    global key
    #global sended_message
    key_space="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    key=random.sample(key_space,16) 
    key=''.join(key)

def padPKCS7(p, block_length):#填充
    pad = block_length - (len(p) % block_length)
    p=p+chr(pad)*pad
    #print len(p)
    return p

def CBC_MAC(key, iv, p):#计算MAC
    cipher = AES.new(key, AES.MODE_CBC, iv)
    c = cipher.encrypt(padPKCS7(p, 16))
    return c[-16:]

def strxor(stringa,stringb):#字符串异或
    string=''
    for i in xrange(0,min(len(stringa),len(stringb))):
        string=string+chr(ord(stringa[i])^ord(stringb[i]))
    return string

def server2(m):#服务器
    global iv
    message = m[:-16]
    mac = m[-16:]
    if CBC_MAC(key, iv, message) != mac:
        print 'messgge has been changed'
    else:
        print "verity success"
        #print message
        for pair in message.split('&'):
            k, v = pair.split('=')
            if (k == 'from'):
                sender = v
            elif (k == 'tx_list'):
                trade_lists = v
        trade= [x.split(':') for x in trade_lists.split(';')]
        for x in trade:
            print sender+" pay "+x[1]+"$"+" to "+x[0]

def clinet_2(sender, trade_lists):#客户端
    global sended_message_list
    global iv
    if not re.match('^[A-Za-z]+$', sender):
        print "sender error"
    for x in trade_lists:
        if not re.match('^[A-Za-z]+$', x[0]):
            print "recepter error"
        elif not re.match('^[0-9]*\.[0-9]+|[0-9]+$',x[1]):
            print "account error"
        else:
            print "sending"
    recepters = ';'.join([x[0] + ':' + x[1] for x in trade_lists])
    message = 'from=' + sender + '&tx_list=' + recepters
    sended_message = message + CBC_MAC(key, iv, message)
    sended_message_list.append(sended_message)
    server2(sended_message)

def attack_2():#攻击者
    global sended_message_list
    message=sended_message_list[0][:-16]
    mac=sended_message_list[0][-16:]
    clinet_2(mac+"hacker",[["hackera","100"],["hackerb","200"]])
    cheat_message=padPKCS7(message,16)+strxor(mac,sended_message_list[1][:16])+sended_message_list[1][16:]
    server2(cheat_message)


if __name__ == '__main__':
    key_generate()
    clinet_2("Alice",[["Boba","100"],["Bobb","100"]])#客户端正常发送消息
    attack_2()#攻击者模拟劫持，并再次发送一次消息，利用相同异或时低效的特性，将第二个消息与MAC异或后附在消息一之后，伪造能被认可的消息