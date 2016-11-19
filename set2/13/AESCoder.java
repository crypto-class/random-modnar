import java.security.Key;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import java.io.*;

public abstract class AESCoder{
	//static String filePath = "C:\\Users\\songc\\Desktop\\25.txt";
	public static final String KEY_ALGORITHM = "AES";
	public static final String CIPHER_ALGORITHM_CTR = "AES/CTR/NoPadding";
	public static final String CIPHER_ALGORITHM_ECB = "AES/ECB/PKCS5Padding";
	public static final String CIPHER_ALGORITHM_CBC = "AES/CBC/PKCS5Padding";
	//public static Key realKey;
	//public static IvParameterSpec realiv;

	static Key toKey(byte[] key){

		SecretKey secretKey = new SecretKeySpec(key, KEY_ALGORITHM);
		return secretKey;
	}

	/*static void setIV(byte[] iv){
		IvParameterSpec ivSpec = new IvParameterSpec(iv);
		realiv = ivSpec;
	}*/

	public static byte[] decryptCTR(byte[] data, byte[] key, byte[] iv) throws Exception{
		Key k = toKey(key);
		IvParameterSpec ivSpec = new IvParameterSpec(iv);
		Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM_CTR);
		cipher.init(Cipher.DECRYPT_MODE, k, ivSpec);
		return cipher.doFinal(data);
	}

	public static byte[] encryptCTR(byte[] data, byte[] key, byte[] iv) throws Exception{
		Key k = toKey(key);
		IvParameterSpec ivSpec = new IvParameterSpec(iv);
		Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM_CTR);
		cipher.init(Cipher.ENCRYPT_MODE, k, ivSpec);
		return cipher.doFinal(data);
	}

	public static byte[] decryptECB(byte[] data, byte[] key) throws Exception{
		Key k = toKey(key);
		//IvParameterSpec ivSpec = new IvParameterSpec(iv);
		Cipher cipher = Cipher.getInstance("AES/ECB/Nopadding");
		cipher.init(Cipher.DECRYPT_MODE, k);
		System.out.println(data.length);
		return cipher.doFinal(data);
	}
	
	public static byte[] encryptECB(byte[] data, byte[] key) throws Exception{
		Key k = toKey(key);
		//IvParameterSpec ivSpec = new IvParameterSpec(iv);
		Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM_ECB);
		cipher.init(Cipher.ENCRYPT_MODE, k);
		return cipher.doFinal(data);
	}
	
	public static byte[] encryptCBC(byte[] data, byte[] key, byte[] iv) throws Exception{
		Key k = toKey(key);
		IvParameterSpec ivSpec = new IvParameterSpec(iv);
		Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM_CBC);
		cipher.init(Cipher.ENCRYPT_MODE, k, ivSpec);
		return cipher.doFinal(data);
	}

	public static byte[] initKey() throws Exception{
		KeyGenerator kg = KeyGenerator.getInstance(KEY_ALGORITHM);
		kg.init(128);
		SecretKey secretKey = kg.generateKey();
		return secretKey.getEncoded();
	}
}