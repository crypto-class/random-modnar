麻烦链接上cryptlib.lib库

对相应的题目编译即可运行
可能输出是.out文件或者直接终端输出

## challenges1 - 将十六进制转为 base64 编码

- 16进制转字符串
- 字符串base64解码

## challenge2 - 固定长度的异或

- 异或即可

## challenge3 - 单字节异或

- 解密后字符是否合法
- 遍历可能字母看看异或后是否有频率词the出现

## challenge4 - 侦查单字节异或

- 遍历可能的ascii
- 遍历所有行
- 担心有换行符的存在可以将换行符去掉链接
- 看看是否有所有字符都是有效字符
- 找出有the的明文

## challenge5 - 进行密钥重复的异或

异或加密

明文是 `Burning 'em, if you ain't quick and nimble I go crazy when I hear a cymbal`

密码是 `ICE`

## challenge6 - 攻破密钥重复的异或

- 猜测密钥长度，相同字母加密后隔密钥长度的倍数距离是相等的，看碰撞数
- 选出最高的碰撞数
- 查找候选字母，统计频率，看和标准频率最接近的是哪个字母
- 依次循环找到密钥

## challenge7 - ECB 模式的 AES 加密

- 破解即可

## challenge8 - 找到 ECB 模式的 AES 加密

- ECB模式容易导致不同组可能会有相同的密文，通过这一特性统计即可
- 计算重叠个数
- 找到结果