package org.cryptopals.set2;

import java.security.InvalidKeyException;
import java.security.Key;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.util.Date;
import java.util.Random;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import org.cryptopals.set2.util;


public class eleven {
	
	public static String CIPHER_ALG = "AES/ECB/NoPadding";
	public static String KEY_ALG = "AES";
	
	
	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		
	
		
		
		byte[] plaintext = "hello wrold".getBytes();
		byte[] ciphertext = encryptByCBCAndECBHalf(plaintext);
		//加密，一半一半
		System.out.println(new String(ciphertext));
	}
	
	public static byte[] encryptByCBCAndECBHalf(byte[] in) throws Exception{
		
		boolean flag = false;
		byte[] inBytes = appendRandomBytes(in);
		//在头和尾加入5-10个随机byte
		byte[] s1 = new byte[inBytes.length/2];
		byte[] s2 = new byte[inBytes.length-s1.length];
		System.arraycopy(inBytes, 0, s1, 0, inBytes.length/2);
		System.arraycopy(inBytes, inBytes.length/2, s2, 0, s2.length);
		//先分然后padding
		
		Date d = new Date();
		Random r = new Random(d.getTime());
		byte[] out1,out2;
		byte[] keyBytes = genAESKey();
		
		//下面利用随机数选择哪个加密
		//1是ebc在前，否则cbc在前
		if(r.nextInt(2) == 1){
			flag=true;
			out1 = encryptByRandomKeyInEBCMode(s1,keyBytes);
			out2 = encryptByRandomKeyInCBCMode(s2,genAESKey(),keyBytes);
		}else{
			out1 = encryptByRandomKeyInEBCMode(s2,keyBytes);
			out2 = encryptByRandomKeyInCBCMode(s1,genAESKey(),keyBytes);
		}
		
		if(flag){
			return ((new String(out1))+(new String(out2) )+"1").getBytes();
		}else{
			return ((new String(out1))+(new String(out2) )+"0").getBytes();
		}
	}
	
	//在前面和后面加入随机数
	public static byte[] appendRandomBytes(byte[] in){
		Date d = new Date();
		Random r = new Random(d.getTime());
		//before
		int a = getIntInRange(r);
		byte[] tmp = new byte[a];
		r.nextBytes(tmp);
		//after
		int b = getIntInRange(r);
		byte[] tmp1 = new byte[a];
		r.nextBytes(tmp1);
		byte[] str = new byte[tmp.length+tmp1.length+in.length];
		//将所有的数据copy到一个中间去
		System.arraycopy(tmp, 0, str, 0, tmp.length);
		System.arraycopy(in, 0, str, tmp.length, in.length);
		System.arraycopy(tmp1, 0, str, tmp.length+in.length, tmp1.length);
		return str;
	}
	
	//生成AES的密钥
	public static byte[] genAESKey() throws NoSuchAlgorithmException{
		KeyGenerator kg = KeyGenerator.getInstance(KEY_ALG);
		SecretKey sk = kg.generateKey();
		return sk.getEncoded();
	}
	//将字节密钥变成密钥对象
	private static Key toKey(byte[] keyStr){
		SecretKey sk = new SecretKeySpec(keyStr,KEY_ALG);
		return sk;
	}
	
	public static byte[] encryptByRandomKeyInEBCMode(byte[] in,byte[] keyBytes) throws Exception{
		//EBC模式
		//先padding，然后加密
		byte[] inBytes;
		int len;
		//padding
		if(in.length%16 != 0){
			len = 16*(in.length/16+1);
			inBytes = util.padding(in, len);
		}else{
			inBytes = in;
		}
	//加密函数
		return AESEBCEncrypto(inBytes,keyBytes);
		
	}
	
	public static byte[] encryptByRandomKeyInCBCMode(byte[] in,byte[] iv,byte[] keyBytes) throws Exception{
		byte[] inBytes;
		int len;
		//padding
		if(in.length%16 != 0){
			len = in.length + (16 - in.length%16);
			inBytes = util.padding(in, len);
		}else{
			inBytes = in;
		}
	
		
		return AESCBCEncrypto(inBytes,keyBytes,iv);
	}
	
	public static byte[] AESEBCEncrypto(byte[] inBytes,byte[] key) throws Exception{
		//EBC模式的加密函数
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
	
	public static byte[]  AESCBCEncrypto(byte[] inBytes,byte[] key,byte[] iv) throws Exception{
		//CBC模式的加密
		Cipher c = getCipher(key);
		
		byte[] tmp = new byte[16];
		byte[] results = new byte[inBytes.length];
		
		//第一个分组
		giveValues(tmp, iv, 0);
		util.xorBytes(inBytes, tmp, 0);
		tmp = c.doFinal(tmp);
		giveValues(results, tmp, 0);
		//循环
		int re = 16;
		while(inBytes.length-re > 0){
			util.xorBytes(inBytes, tmp, re);
			tmp = c.doFinal(tmp);
			giveValues(results, tmp, re);
			re+=16;
		}
		return results;
	}
	
	
	
	public static void giveValues(byte[] toHere,byte[] tmp,int k){
		for(int i = 0; i<16;i++){
			toHere[k+i]=tmp[i];
		}
	}
	
	private static Cipher getCipher(byte[] keyBytes) throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException{
		Cipher cipher = Cipher.getInstance(CIPHER_ALG);
		Key key = toKey(keyBytes);
		cipher.init(Cipher.ENCRYPT_MODE, key);
		return cipher;
	}
	
	
	
	private static int getIntInRange(Random r){
		//在5-10之间生成一个随机数
		while(true){
			int a = r.nextInt(11);
			if(a>=5){
				return a;
			}
		}
	}
	
	


}
