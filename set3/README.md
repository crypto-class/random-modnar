Set3中需要额外安装的库函数：pycrypto

>在以下提到的脚本中，均在py文件内写有注释，以下部分仅供

# Challenge17

题目要求使用pkcs7的方式，在给出的10个字符串中随机选取一个，对该字符串进行padding后加密，并解密后验证padding

其中定义class：AESCipher 主要包含两个函数及两个self变量。

*	其中``GetRandomStr``函数的主要功能为随机选取字符串，并对这个字符串进行padding，最终对padding结果使用AES的CBC模式加密，并按照题目要求返回msg和iv值
*	其中``DecryptCheckPadding``函数的主要功能为对密文进行解密，并且检验padding的方式是否正确，检验padding的方式主要由上文中提到的``pkcs7unpad``函数提供。

演示输出结果：第一个为正确结果，第二个对padding进行了篡改，因此输出的为False结果

# Challenge18

题目要求为对给出的字符串，并按照题中所给出的参数，使用AES的CTR模式对字符串进行解密。

演示输出结果：Yo, VIP Let's kick it Ice, Ice, baby Ice, Ice, baby

# Challenge19

题目要求，按照上一题中使用的方法，生成随机的AES密钥，并将题目中给出的message进行base64解码，在进行AES的CTR模式加密，并将并通过语义分析的方法进行猜测密钥，将原文恢复。

按照题目中给出的方法，由于该方法为基于对语义的尝试的方法，因此需要尝试多次才能够得出正确的结果，存在一定误差率，但本脚本使用的方法使误差仅存在于大小写之间，只需尝试几次便可得到正确结果。

验证方法：脚本中已提供

# Challenge20

题目要求同上一题相同，将文件中的内容进行相同模式的处理，只有解密的方式不同，要求通过分析频率的方式恢复key。

现有代码基本上为100%的正确率。

如果觉得正确率不稳定的话，可以更改函数中注释指出的部分增加append，从而提高正确率。

验证方法：可以将网站中的文本下载并进行base64解码同该脚本输出的内容进行比较。

# Challenge21

题目要求按照Wikipedia编写MT19937随机数

其中演示函数为main函数，可以输出random值，注：MT19937算法为伪随机数生成算法，每次重新生成顺序相同。

# Challenge22

题目要求，按照题目中的操作方式生成RNG，并对MT19937随机数进行恢复。

由于原题目的方式主要采用时间上的延时，在脚本中稍作更改采用更改数值的方式控制时间延时（已在注释中说明）

验证方式：脚本中已提供

# Challenge23

题目要求，主要为逆向操作MT19937算法中的temper算法，将temper运算的结果恢复成运算前的数值。

在脚本中采用了两种方法进行计算，其中具体操作方法已在注释中列出例子。

验证方式：脚本中已提供

# Challenge24

题目要求，通过按照题目的要求，创建一个MT19937流密钥加密函数，并对其key进行恢复。在测试成功后，用同样的方式对时间戳进行加密及恢复。

其中题目中要求生成随机的prefix的函数由``generatePrefix``提供。此外创建的MT19937加密函数具体操作方式已在注释中写出

本机测试至少需要21.2s

验证方式：脚本中已提供