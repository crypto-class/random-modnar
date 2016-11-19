麻烦链接上cryptlib.lib库

对相应的题目编译即可运行
可能输出是.out文件或者直接终端输出

## challenge33 - 实现 Diffie-Hellman 算法

实现 Diffie-Hellman

## challenge34 - Diffie-Hellman 的中间人攻击

- 生成各自的私钥
- 协商 p,g
- 中间人也模拟上述协商过程
- 协商出来的共同密钥g^(a*b)
- 中间人攻击完成

## challenge35 实现 DH 协商,并通过恶意参数 g 攻破之

协商过程同上一节

攻击过程如下

```
when g = 1
KA = 1^a mod p = 1
KB = 1^b mod p = 1
Key = 1^a^b mod p = 1
You can use key = 1 to attack

when g = p
KA = p^a mod p = 0
KB = p^b mod p = 0
Key = p^a^b mod p = 0
You can use key = 0 to attack

when g = p - 1
KA = (p-1)^a mod p = 1 || -1
KB = (p-1)^b mod p = 1 || -1
Key = (p-1)^a^b mod p  = 1 || -1
KA and KB maybe different
You can use key = 1 || p -1  to attack
```

## challenge36 - Implement SRP (Secure Remote Password)

过程分析：

1. 首先协商好 N, g = 2, k = 3, email and password

2. server 随机生成salt和密文进行连接后 hash = x 利用hash值生成可传递的 v = g**x % N， 保存下来v用来以后对比

3. client 随机生成a, DH协议生成 A = g ** a %N

4. server 把salt给client， 顺便生成B = kv + g**b % N

5. 对A|B进行hash

6. client得到salt后跟自己输入的password串起来后hash = x 计算 S = (B - k * g**x)**(a + u * x) % N 生成 K = SHA256(S) 发送 HMAC-SHA256(K, salt)

7. server计算 S = (A * v**u) ** b % N 生成 K = SHA256(S) 生成HMAC-SHA256(K, salt) 和 client的进行比较返回是否相同

	server的公式 S = (A * v**u) ** b % N = ( (g**a) * (g**x)**u ) ** b % N = g**(a+ux)**b % N

	client的公式 S = (B - k * g**x)**(a + u * x) % N = (k*(g**x) + g**b - k* g**x )**(a + u**x)) % N = (g**b)**(a + u*x) % N = g**b**(a+ux) % N

## challenge37 - 在 key 为0 时破解 SRP

协商过程和上一节类似

主要是攻击过程不同

过程如下

```
send A = 0
you can know S = (A * v**u) ** b % N
so you can send your S = 0 to attack

send A = multiple of N
you can know S = (k*N * v**u)**b %N = 0
the same as A = 0
```

## challenge38 - 通过脱机字典攻击 SRP

字典攻击

跑字典就好了

## challenge39 - 实现 RSA

过程如下

```
生成俩素数 p,q

令N = p*q

phi(N) = (p-1)(q-1)

public key = e, gcd(e,phi) = 1

private key = d, e*d = 1 mod N

cipher = plain^e

plain = cipher^d

```

## challenge40 实现当 E 等于 3 时的 RSA 广播攻击

假如同一的明文用不同的私钥对RSA多次进行加密

那么用 中国剩余定理 很容易进行破解

当然，，前提是e是一样的情况下。。。

```
M^e = C1 mod N1
M^e = C2 mod N2
M^e = C3 mod N3
```
