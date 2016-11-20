package org.cryptopals.set2;

import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.util.Date;
import java.util.Random;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;

import org.bouncycastle.util.encoders.Base64;

public class forteen {
	public static String plaintext = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"+"aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"+"dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"+"YnkK";
	public static String CIPHER_ALG = "AES/ECB/PKCS5Padding";
	public static String KEY_ALG = "AES";
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//
		byte[] in = decode(plaintext);
		//先base64解密
		byte[] appending = randomBytes();
		
		try {
			System.out.println(new String(tryToGetPlaintext(in,appending)));
		} catch (InvalidKeyException | NoSuchAlgorithmException | NoSuchPaddingException | NoSuchProviderException
				| IllegalBlockSizeException | BadPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		
	}
	
	public static byte[] tryToGetPlaintext(byte[] in,byte[] randomBytes) throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, NoSuchProviderException, IllegalBlockSizeException, BadPaddingException{
		//get明文的主入口
		int[] start = getStartAndShiftIndex(in, randomBytes);
		//获取需要填充的数量个组数，0-字节数，1-填充数
		byte[] result = new byte[in.length];
		
		int reIndex = 0;//start[0]
		//构造chaRandom，在random的基础上增加到分组的倍数
		byte[] chaRandom = new byte[randomBytes.length+start[1]];
		System.arraycopy(randomBytes, 0, chaRandom, 0, randomBytes.length);
		for(int i = randomBytes.length;i<chaRandom.length;++i){
			chaRandom[i]=(byte)'A';
		}
		int loop;
		//计算需要循环计算的次数，每次get一个分组的明文
		if(in.length%16 == 0){
			loop = in.length/16;
		}else{
			loop = in.length/16+1;
		}
		for(int i = 0;i<loop;++i){
			int z=i*16+start[0];
			//start[0],开始的字节数
			//入口去，开始一分组一个分组的get
			byte[] temp = get1Block(in, chaRandom, z);
			for(int k = 0 ; k < 16 && reIndex < in.length;reIndex++,k++){
				result[reIndex] = temp[k];
			}
		}
		return result;
		
		
		
	}
		
	public static byte[] get1Block(byte[] plaintext,byte[] random,int shift) throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException, NoSuchProviderException{
			//shift的作用就是用来确定明文构造的位置的，shift里面有一个random.length，
			//初始化test
		byte[] test =  new byte[16];
		for(int i = 0;i<16;++i){
			test[i]=(byte)'A';
		}
		int testIndex = test.length-1;
		//test的下标，是要搞到明文的位置文
		
		
		//构造发送明文
		byte[] plain = new byte[random.length+plaintext.length+16];//+16
		byte[] result = new byte[16];
		int reIndex = 0;
		//随机数和部分控制的数，补成满组
		//初始化plain
		System.arraycopy(random, 0, plain, 0,random.length);
		for(int i = random.length;i < shift+15 ;++i){
			plain[i]=(byte)'A';
		}
		
		for(;reIndex<16;reIndex++,testIndex--){
			//初始化test
			for(int k=0;k<reIndex;++k){
				test[k+testIndex]=result[k];
			}
			
			//修改,每次更改完都需要有数据往前移，所以这里得用变量，然后每次get到数据都要往前整体移动一字节
			for(int k = shift+testIndex,j=0;shift-random.length+j<plaintext.length ;++k,++j){
				plain[k]=plaintext[shift-random.length+j];
			}
			//一个字节一个字节的get
			result[reIndex] = getLast1ByteIn1Block(plain,test,shift);
		}
		return result;
	}
	
	public static byte getLast1ByteIn1Block(byte[] plaintext ,byte[] test,int len) throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException, NoSuchProviderException{
		//len表示要爆破的长度，只爆破改长度最后一个字节
		Cipher c = util.getCipher(util.genAESKey());
		byte[] plain = util.autoPKCS7Padding(plaintext);
		byte[] res = c.doFinal(plain);
		boolean flag = false;
		
		
		for(int i = 0 ;i<256 ; ++i){
			test[15]=(byte)i;
			//每次更改test的最后一个字节
			byte[] temp = c.doFinal(test);
			//测试整个块是否相同
			for(int k = 0;k<16;++k){
				if(temp[k] != res[k+len]) break;
				if(k == 15) flag = true;
			}
			if(flag){
				return (byte)i;
			}
		}
		return (byte)0;
	}
		

	public static int[] getStartAndShiftIndex(byte[] in,byte[] randomBytes) throws IllegalBlockSizeException, BadPaddingException, InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, NoSuchProviderException{
		//定位randomBytes耗费的block的长度
		Cipher c = util.getCipher(util.genAESKey());
		byte[] fixedPadding = new byte[32];
		for(int i = 0 ;i<32 ;++i){
			fixedPadding[i] = (byte)'A';
		}
		byte[] inBytes = new byte[in.length+32+fixedPadding.length];
		
		//拷贝
		System.arraycopy(randomBytes, 0, inBytes, 0, randomBytes.length);
		System.arraycopy(fixedPadding, 0, inBytes, randomBytes.length, fixedPadding.length);
		System.arraycopy(in, 0, inBytes, randomBytes.length+fixedPadding.length, in.length);
		
		byte[] fixedResult = c.doFinal(fixedPadding);
		byte[] allResult = c.doFinal(inBytes);
		int[] start = new int[2];
		
		start[0] = whereEqual(allResult, fixedResult);
		
		for(int i = 31;i>0;--i){
			byte[] fixedPadding1 = new byte[i];
			for(int k = 0 ;k<i ;++k){
				fixedPadding[k] = (byte)'A';
			}
			byte[] inBytes1 = new byte[in.length+randomBytes.length+fixedPadding1.length];
			System.arraycopy(randomBytes, 0, inBytes1, 0, randomBytes.length);
			System.arraycopy(fixedPadding, 0, inBytes1, randomBytes.length, fixedPadding1.length);
			System.arraycopy(in, 0, inBytes1, randomBytes.length+fixedPadding1.length, in.length);
			
			byte[] fixedResult1 = c.doFinal(fixedPadding);
			byte[] allResult1 = c.doFinal(inBytes1);
			if(whereEqual(allResult1,fixedResult1) == -1){
				start[1] = i-15;
//				System.out.println("start1:"+start[1]);
				break;
			}
		}
		return start;
	}
	//找到一个分组完全相等的下标
	public static int whereEqual(byte[] one,byte[] two){
		int start = -1;
		int oneIndex=0;
		for(int i = 0;i+oneIndex<one.length;){
			if(one[oneIndex+i] != two[i]){
				i=0;
				oneIndex+=16;
				continue;
			}else if(one[oneIndex+i] == two[i] && i==15){
				start=oneIndex;
				break;
			}else{
				i++;
			}
		}
		return start;
	}
	
	//生成随机数量的随机数，限制在了32个以内
	public static byte[] randomBytes(){
		Date d = new Date();
		Random r = new Random(d.getTime());
		int a = r.nextInt(32);
		byte[] b = new byte[a];
		r.nextBytes(b);
		return b;
	}
	
	public static byte[] decode(String input){
		
		return Base64.decode(input);
	}


}
