# Challenge 32
## 服务端
由于PHP受环境影响过大  
服务端使用高性能JAVA编写  
源码使用Maven编译  
InsecureCompare 函数  
```java
private boolean InsecureCompare(String s0, String s1) throws Exception{
    for(int i=0; i < s0.length();i++){
        if(!s0.substring(i, i+1).equals(s1.substring(i, i+1))) return false;
        Thread.sleep(5);
    }
    return s0.length() == s1.length();
}
```
## python 客户端调试输出
    4  
    4f  
    4fa  
    4fa6  
    4fa62  
    4fa621  
    4fa621b  
    4fa621b7  
    4fa621b7e  
    4fa621b7e3  
    4fa621b7e3c  
    4fa621b7e3c3  
    4fa621b7e3c30  
    4fa621b7e3c302  
    4fa621b7e3c3029  
    4fa621b7e3c30296  
    4fa621b7e3c30296b  
    4fa621b7e3c30296bd  
    4fa621b7e3c30296bde  
    4fa621b7e3c30296bde9  
    4fa621b7e3c30296bde94  
    4fa621b7e3c30296bde943  
    4fa621b7e3c30296bde9433  
    4fa621b7e3c30296bde94335  
    4fa621b7e3c30296bde943351  
    4fa621b7e3c30296bde9433519  
    4fa621b7e3c30296bde943351  
    4fa621b7e3c30296bde9433510  
    4fa621b7e3c30296bde943351  
    4fa621b7e3c30296bde9433519  
    4fa621b7e3c30296bde94335196  
    4fa621b7e3c30296bde94335196a  
    4fa621b7e3c30296bde94335196ad  
    4fa621b7e3c30296bde94335196ade  
    4fa621b7e3c30296bde94335196ade3  
    4fa621b7e3c30296bde94335196ade34  
    4fa621b7e3c30296bde94335196ade342  
    4fa621b7e3c30296bde94335196ade3426  
    4fa621b7e3c30296bde94335196ade342  
    4fa621b7e3c30296bde94335196ade34  
    4fa621b7e3c30296bde94335196ade341  
    4fa621b7e3c30296bde94335196ade34  
    4fa621b7e3c30296bde94335196ade342  
    4fa621b7e3c30296bde94335196ade3426  
    4fa621b7e3c30296bde94335196ade34266  
    4fa621b7e3c30296bde94335196ade342660  
    4fa621b7e3c30296bde94335196ade34266  
    4fa621b7e3c30296bde94335196ade3426  
    4fa621b7e3c30296bde94335196ade34262  
    4fa621b7e3c30296bde94335196ade342621  
    4fa621b7e3c30296bde94335196ade342621d  
    4fa621b7e3c30296bde94335196ade342621  
    4fa621b7e3c30296bde94335196ade3426210  
    4fa621b7e3c30296bde94335196ade342621  
    4fa621b7e3c30296bde94335196ade3426219  
    4fa621b7e3c30296bde94335196ade34262190  
    4fa621b7e3c30296bde94335196ade3426219  
    4fa621b7e3c30296bde94335196ade342621  
    4fa621b7e3c30296bde94335196ade342621d  
    4fa621b7e3c30296bde94335196ade342621d2  
    4fa621b7e3c30296bde94335196ade342621d2d  
    ====================  
    end: 4fa621b7e3c30296bde94335196ade342621d2d6  
    [Finished in 763.9s]  
