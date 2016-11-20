package org.cryptopals.set2;

import java.security.InvalidKeyException;
import java.security.Key;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;


	public class twelve {
		
		public static String plaintext = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"+"aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"+"dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"+"YnkK";
		public static String CIPHER_ALG = "AES/ECB/NoPadding";
		public static String KEY_ALG = "AES";
		
		
		
		public static int blockSize = 0;
		
		
		public static void main(String[] args) {
			// TODO Auto-generated method stub
	
			byte[] in = util.decode(plaintext).getBytes();
			try {
				
				getBlockSize(in);
				//得到AES的块的大小
				if(isECB()){
					//是否是ECB模式
					byte[] re = tryToGetPlaintext(in);
					//get明文信息
					System.out.println(new String(re));
					//将明文信息打印到控制台
					
				}else{
					//不是EBC模式的话不进行任何处理
					System.out.println("sorry, it's no ECB mode");
				}
				
				
				
				
			} catch (InvalidKeyException | NoSuchAlgorithmException | NoSuchPaddingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalBlockSizeException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (BadPaddingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
		
		public static byte[] tryToGetPlaintext(byte[] in) throws NoSuchAlgorithmException, Exception{
			//处理的入口函数，就是计算一下，如果一次get一个组的明文，要循环多少次能get完。然后每次都把控制权给了get1Block()
			byte[] result = new byte[in.length];
			int reIndex = 0;
			int loop;
			if(in.length%16 == 0){
				loop = in.length/16;
			}else{
				loop = in.length/16+1;
			}
			for(int i = 0;i<loop;++i){
				byte[] temp = get1Block(in, i*16);
				//每次get一个块的大小都靠这个函数
				for(int k = 0 ; k < 16 && reIndex < in.length;reIndex++,k++){
					result[reIndex] = temp[k];
				}
			}
			return result;
			
		}
		public static byte[] get1Block(byte[] plaintext,int shift) throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException, NoSuchProviderException{
			//传入明文，和偏移量，偏移量的意思是从这个位置开始爆破明文，小端序
			//初始化test
			byte[] test =  new byte[16];
			for(int i = 0;i<16;++i){
				test[i]=(byte)'A';
			}
			int testIndex = test.length-1;
			
			byte[] result = new byte[16];
			int reIndex = 0;
			
			for(;reIndex<16;reIndex++,testIndex--){
				//初始化test测试数据，每次都要及时更新已经算出来的数据
				for(int k=0;k<reIndex;++k){
					test[k+testIndex]=result[k];
				}
			
				//初始化明文构造
				byte[] p = new byte[plaintext.length+16];
				for(int k =0;k<testIndex+shift;k++){
						p[k]=(byte)'A';
				}
				//及时更新明文的构造，相当于每次出来一个字节，然后把那个字节放入整个构造中，然后再重新构造
				for(int k =testIndex+shift,j=0;j+shift < plaintext.length ;++k,j++){//&& testIndex+k < p.length
					p[k]=plaintext[j+shift];
				}
				//get到一个字节的数据
				result[reIndex] = getLast1ByteIn1Block(p,test,shift);
			}
			return result;
		}
		
		
		public static byte getLast1ByteIn1Block(byte[] plaintext ,byte[] test,int shift) throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException, NoSuchProviderException{
			//get一个字节的明文
			//shift尾爆破的偏移位置，只爆破改长度最后一个字节即shift+15，也就是所说的小端序
			
			Cipher c = getCipher(util.genAESKey());
			byte[] plain = util.autoPKCS7Padding(plaintext);
			byte[] res = c.doFinal(plain);
			boolean flag = false;
			for(int i = 0 ;i<256 ; ++i){
				test[15]=(byte)i;
				//每次更改test的最后一个字节
				byte[] testCipher = c.doFinal(test);
				for(int k = 0;k<16;++k){
					//循环判断是否byte数组是否相等
					if(testCipher[k] != res[k+shift]){
						break;
					}
					if(k == 15){
						flag = true;
					}
				}
				if(flag){
					return (byte)i;
				}
			}
			//如果跑破不出来返回全1，当然，不可能爆破不出来
			return (byte)0xff;
		}
		
		
		public static boolean isECB() throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException, NoSuchProviderException{
			//判断是否是EBC模式，得到blocksize->用两个相同数据的分组加密，如果密文相同就是，不相同就不是
			if(blockSize == 0){
				getBlockSize("hello1234567893125464f68adf".getBytes());
			}
			String test = "";
			for(int i = 0;i<2*blockSize;++i){
				test+="A";
			}
			Cipher c = getCipher(util.genAESKey());
			byte[] out = c.doFinal(test.getBytes());
			boolean flag = true;
			for(int i = 0;i<blockSize;++i){
				if(out[i] != out[blockSize+i]){
					flag=false;
					break;
				}
			}
			return flag;
		}
		
		
		
		public static int getBlockSize(byte[] in) throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, NoSuchProviderException{
			//得到AES分组大小，尝试一个一个增加，不使用任何padding，如果没有报错，肯定就是了。两次不报错的差值就是分组的大小
			Cipher c = getCipher(util.genAESKey());
			int b=0,d=0;
			boolean flag=false;
			for(int i = 0;true;){
				//往前面i个A的函数
				byte[] use = addABefore(in,i);
				try {
					c.doFinal(use);
				} catch (IllegalBlockSizeException | BadPaddingException e) {
					//报错的时候循环继续
					++i;
					continue;
				}
				if(flag){
					d=i;
					break;
				}else{
					b=i;
					flag = true;
				}
				++i;
			}
			//将全局变量设置一下
			blockSize=d-b;
			return d-b;
		}
	
		
		public static byte[] AESEBCEncrypto(byte[] inBytes,byte[] key) throws Exception{
			//实习AES,CBC模式加密
			Cipher c = getCipher(key);
			byte[] tmp = new byte[16];
			byte[] results = new byte[inBytes.length];
			int re = 0;
			while(inBytes.length-re > 0){
				tmp = c.doFinal(tmp);
				giveValues(results, tmp, re);
				re+=16;
			}
			return results;
		}
		
		private static Cipher getCipher(byte[] keyBytes) throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, NoSuchProviderException{
			Cipher cipher = Cipher.getInstance(CIPHER_ALG);
			Key key = toKey(keyBytes);
			cipher.init(Cipher.ENCRYPT_MODE, key);
			return cipher;
		}
		
		public static void giveValues(byte[] toHere,byte[] tmp,int k){
			//将tmp的16个数值给toHere以k为偏移
			for(int i = 0; i<16;i++){
				toHere[k+i]=tmp[i];
			}
		}
		
		private static Key toKey(byte[] keyStr){
			SecretKey sk = new SecretKeySpec(keyStr,KEY_ALG);
			return sk;
		}
		
		public static byte[] addABefore(byte[] data,int len){
			//添加len个A在数据前面
			String pad = "";
			for(int i = 0 ; i< len;++i){
				pad+="A";
			}
			byte[] one = pad.getBytes();
			byte[] res = new byte[one.length+data.length];
			for(int i =0 ; i<one.length;++i){
				res[i]=one[i];
			}
			for(int i = 0 ;i < data.length;++i){
				res[i+one.length]=data[i];
			}
			return res;
			
		}
		
		
}
