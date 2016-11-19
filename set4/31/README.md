# Challenge 31
## 服务端
服务端采用简单的PHP  
insecure_compare函数  
```php
function insecure_compare( $cmp_str0, $cmp_str1){
    for( $i = 0; $i < strlen($cmp_str0); $i++){
        if( $cmp_str0[$i] != $cmp_str1[$i]) return false;
        usleep(50000); // Sleep 50 ms
    }
    return strlen($cmp_str0) == strlen($cmp_str1);
}
```
## python 调试输出
    e  
    eb  
    eb7  
    eb78  
    eb782  
    eb782f  
    eb782f7  
    eb782f7c  
    eb782f7c2  
    eb782f7c22  
    eb782f7c228  
    eb782f7c2280  
    eb782f7c2280c  
    eb782f7c2280c9  
    eb782f7c2280c9c  
    eb782f7c2280c9cd  
    eb782f7c2280c9cd8  
    eb782f7c2280c9cd87  
    eb782f7c2280c9cd871  
    eb782f7c2280c9cd8718  
    eb782f7c2280c9cd87183  
    eb782f7c2280c9cd871838  
    eb782f7c2280c9cd871838a  
    eb782f7c2280c9cd871838a3  
    eb782f7c2280c9cd871838a31  
    eb782f7c2280c9cd871838a31d  
    eb782f7c2280c9cd871838a31dc  
    eb782f7c2280c9cd871838a31dc5  
    eb782f7c2280c9cd871838a31dc54  
    eb782f7c2280c9cd871838a31dc540  
    eb782f7c2280c9cd871838a31dc5405  
    eb782f7c2280c9cd871838a31dc5405d  
    eb782f7c2280c9cd871838a31dc5405db  
    eb782f7c2280c9cd871838a31dc5405db0  
    eb782f7c2280c9cd871838a31dc5405db04  
    eb782f7c2280c9cd871838a31dc5405db049  
    eb782f7c2280c9cd871838a31dc5405db04  
    eb782f7c2280c9cd871838a31dc5405db040  
    eb782f7c2280c9cd871838a31dc5405db04  
    eb782f7c2280c9cd871838a31dc5405db049  
    eb782f7c2280c9cd871838a31dc5405db0494  
    eb782f7c2280c9cd871838a31dc5405db0494c  
    eb782f7c2280c9cd871838a31dc5405db0494cd  
    ====================  
    end: eb782f7c2280c9cd871838a31dc5405db0494cd5  
    [Finished in 399.5s]  
