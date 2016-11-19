# Set2_13

### 题目要求

1. 实现`parse(string)`函数，将形如`suej=kdi&keod=dd&kek=dd`的字符串转换成以下形式：

```
{
	suej: kdi
	keod: dd
	kek: dd
}
```

2. 实现`profile_for()`函数，将一个电子邮件地址转换为以下形式。例如：

```
{
	email: sfsfsf@qq.com
	uid: 10
	role: user
}
```

3. 实现`encoded()`函数，将`profile_for()`函数的返回值转换成如下格式：

```
	email=sfsfsf@qq.com&uid=10&role=userz
```

4. 先将`profile_for()`及其产生的密文提供给攻击者，那么他应怎样做，才能构造出解密后`role`值为`admin`的字符串？

### 步骤

1. 输入电子邮箱`sfsfsf@qq.com`,产生相应字符串：
	
```
email=sfsfsf@qq.com&uid=10&role=user
```

将该字符串按16Bytes一组分割：

```
email=sfsfsf@qq. | com&uid=10&role= | user
```

注意到`role`的值刚好被分割，因此只要在加密后取前两段，并在后拼接admin的密文即可。
2. 输入用来攻击的字符串`sfsfsfsfsfadmin`，产生如下字符串:
	
```
email=sfsfsfsfsfadmin&uid=10&role=user
```

分段：
	
```
email=sfsfsfsfsf | admin&uid=10&rol | e=user
```

这里`admin`之前的字符起到填充作用，使`admin`正好处于一个分段的首位。

3. 截取第一个字符串密文的前两段和第二个字符串密文的第二段，将它们拼接到一起

4. 解密，最终得到字符串：

```
email=sfsfsf@qq.com&uid=10&role=admin&uid=10&rol
```

