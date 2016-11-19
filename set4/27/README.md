# Challenge 27
仅有函数 encrypt(data) decrypt(data) 知道密钥 _key  
decrypt()中  
```python
cipher = AES.new(_key,AES.MODE_CBC,'\x00'*16)
```
使用 '\x00'*16 作为iv，只使用AES分组加密，然后再用iv异或  
## python 调试输出
    iv(key)      : CeFw=Zj1mDRXV=Z@  
    Attacker Key : CeFw=Zj1mDRXV=Z@  
    [Finished in 0.2s]  
