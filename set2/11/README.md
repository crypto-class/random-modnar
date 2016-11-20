## challenge11 - 一个 ECB/CBC 模式检测预言机

- 在头和尾加入5-10个随机byte
- 先分组然后padding
- 生成AES的密钥
- 下面利用随机数选择哪个加密：1是ebc在前，否则cbc在前
