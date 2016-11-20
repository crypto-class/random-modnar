题目链接：https://www.mysterytwisterc3.org/en/challenges/level-ii/hybrid-encryption-i

主要思路：因为n比2的128次方小，因此可以直接将n开三次方根，即能得到密钥，其中需要处理一下精度问题

在解密的时候直接按照题目要求的使用AES的ECB模式进行解密，结果已验证。

结果：
![](hybrid.png)

