package org.cryptopals.set2;

import java.security.InvalidKeyException;
import java.security.Key;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

import org.bouncycastle.util.encoders.Base64;

public class util {
	
	public static String KEY_ALG = "AES";
	public static String CIPHER_ALG = "AES/ECB/PKCS5Padding";

	
	//异或，挨个字节异或，k是one的偏移，结果最后放到tmp里面
	public static void xorBytes(byte[] one,byte[] tmp,int k){
		for(int i = 0;i<16 ;++i){
			tmp[i] ^= one[k+i];
		}
	}
	//实现PKCS#7Padding协议
	public static byte[] autoPKCS7Padding(byte[] data){
		byte[] result;
		if(data.length%16 == 0){
			result = data;
		}else{
			//填充
			result = padding(data,(data.length/16 + 1)*16);
		}
		return result;
	}
	//增加padding到分组倍数的长度
	public static byte[] padding(byte[] str,int len){
		int a = len - str.length;
		char pad = getPadding(a);
		byte[] out = new byte[len];
		for(int i=0; i<str.length;++i){
			out[i]=str[i];
		}
		for(int i=0; i<a;++i){
			out[str.length+i]=(byte)pad;
		}
		return out;
	}
	//得到要填充的数据
	private static char getPadding(int subLen){
		
		switch(subLen){
		case 1:
			return 0x1;
		case 2:
			return 0x2;
		case 3:
			return 0x3;
		case 4:
			return 0x4;
		case 5:
			return 0x5;
		case 6:
			return 0x6;
		case 7:
			return 0x7;
		case 8:
			return 0x8;
		case 9:
			return 0x9;
		case 10:
			return 0xa;
		case 11:
			return 0xb;
		case 12:
			return 0xc;
		case 13:
			return 0xd;
		case 14:
			return 0xe;
		case 15:
			return 0xf;
		}
		return 0;
	}
	//base64编码
	public static String encode(String input){
		
		
		byte[] data = Base64.encode(input.getBytes());
		return new String(data);
	}
	//base64解码
	public static String decode(String input){
		
		byte[] data = Base64.decode(input);
		return new String(data);
	}
	
	public static byte[] genAESKey() throws NoSuchAlgorithmException{
		KeyGenerator kg = KeyGenerator.getInstance(KEY_ALG);
		SecretKey sk = kg.generateKey();
		return sk.getEncoded();
	}
	
	public static Key toKey(byte[] keyStr){
		SecretKey sk = new SecretKeySpec(keyStr,KEY_ALG);
		return sk;
	}
	
	public static Cipher getCipher(byte[] keyBytes) throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, NoSuchProviderException{
		Cipher cipher = Cipher.getInstance(CIPHER_ALG);
		Key key = toKey(keyBytes);
		cipher.init(Cipher.ENCRYPT_MODE, key);
		return cipher;
	}
	
}
