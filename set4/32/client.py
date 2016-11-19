#!/usr/bin/python

'''
File name: client.py
Created by: WRH
Email: o@seceve.com
http://www.cryptopals.com/sets/4/challenges/32
'''

import sys
import httplib
import time

file_name = 'test.png'
host = '127.0.0.1'
ave_time = 5
url = 'http://' + host + '/test'
get = '?file={file_name}&signature={signature}'
ch = '0123456789abcdef'

def request(host,url,get = ''):
    time1 = get_time()
    conn = httplib.HTTPConnection(host);
    conn.request(method = 'GET', url = url + get)
    resp = conn.getresponse()
    conn.close()
    time2 = get_time()
    return resp.status == 200,(time2 - time1)*1000

def get_time():
    if sys.platform == 'win32':
        return time.clock()
    else:
        return time.time()

# Dump
flag = False
tmp_time = 0
signature = ''
while not flag:
    print signature
    for i in ch:
        req_get = get.replace('{file_name}',file_name)\
                     .replace('{signature}',signature+i)
        t = 0.0
        # Early-exit string compares are probably the most common source of cryptographic timing leaks, but they aren't especially easy to exploit. In fact, many timing leaks (for instance, any in C, C++, Ruby, or Python) probably aren't exploitable over a wide-area network at all. To play with attacking real-world timing leaks, you have to start writing low-level timing code. We're keeping things cryptographic in these challenges.
        for ii in range(0,10):
            flag,tmp = request(host,url,req_get)
            t += int(tmp)
        t /= 10
        if abs(t - tmp_time) >= ave_time - ave_time/5:
            signature += i
            tmp_time = t
            break
        else:
            tmp_time = (tmp_time + t)/2
        if i == ch[-1]: # Avoid network delay 
            signature = signature[0:-1]
print '='*20
print 'end: '+signature

# d349270146d4f5773930c8e83c476c48eb9f0463