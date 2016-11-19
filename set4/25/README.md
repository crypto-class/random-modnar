# set4_25

### 题目要求

1. 在一个CRT模式的AES加密系统中，设计一个API，形如`edit(ciphertext, key, offset, newtext)`，它的功能为修改被加密的明文，并将其转换为密文。其中，`ciphertext`为原本密文，`key`为密钥，`offset`为要修改的部分相对于密文第一个字符的偏移量，`newtext`为新的明文。

2. 将以上API提供给攻击者。其中，攻击者能够掌握的参数有`ciphertext`,`offset`,`newtext`。攻击者应该怎样使用以上条件，复原原本的密文？

### 步骤

1. 生成一个全零串，长度同密文相同。

2. 将该全零串作为`newtext`参数传入API中，得到新的加密结果。

3. 将新的加密结果同旧的加密结果异或，即复原明文。

### 原理

CTR加密模式可以简单描述为：		

```
ciphertext = plaintext ^ keystream;
```

如果想要修改明文，则其过程为;

```
newciphertext = ciphertext ^ plaintext ^ newtext;
```

新旧密文相异或，得：

```
ciphertext ^ newciphertext = plaintext ^ newtext;
```

`newtext`被攻击者掌握，因此可轻松复原出原明文。 