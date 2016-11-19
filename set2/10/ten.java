package org.cryptopals.set2;

import org.bouncycastle.util.encoders.Base64;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.security.GeneralSecurityException;
import java.security.InvalidKeyException;
import java.security.Key;
import java.security.NoSuchAlgorithmException;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

public class ten {
	
	public static String KEY_ALG = "AES";
	public static String CIPHER_ALG = "AES/ECB/NoPadding";
	
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		String fs = "G:\\college_study\\������\\�ִ�����ѧ\\cryptopals\\set2\\10.txt";
		String ks = "YELLOW SUBMARINE";
		try {
			//���ļ����н��ܣ�fs���ļ�����ks����Կ��genZeroIv()������ȫ���IVֵ
			byte[] result = AESDeFile(fs, ks, genZeroIv());
			System.out.println(encode(result));
			
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static byte[] AESDeFile(String fStr,String keyStr,String iv) throws Exception{
		//�����ļ�����
		String b64Str = readAll(fStr);
		//base64����
		byte[] in = decode(b64Str);
		byte[] inBytes;
		int len;
		//padding���
		if(in.length%16 != 0){
			len = in.length + (16 - in.length%16);
			String some = new String(in);
			inBytes = padding(some, len).getBytes();
		}else{
			inBytes = in;
		}
		
		return AESCBCDecrypto(inBytes,keyStr.getBytes(),iv.getBytes());
	}
	
	public static byte[]  AESCBCDecrypto(byte[] inBytes,byte[] key,byte[] iv) throws Exception{
		//��������
		Cipher c = getCipher(key);
		//������Կ�����������
		byte[] tmp = new byte[16];
		byte[] results = new byte[inBytes.length];
		
		//��һ������
		giveValues(tmp, iv, 0);
		xorBytes(inBytes, tmp, 0);
		tmp = c.doFinal(tmp);
		giveValues(results, tmp, 0);
		//ѭ������ʣ�µ�
		int re = 16;
		while(inBytes.length-re > 0){
			xorBytes(inBytes, tmp, re);
			tmp = c.doFinal(tmp);
			giveValues(results, tmp, re);
			re+=16;
		}
		return results;
	}
	
	
	private static String padding(String str,int len){
		int a = len - str.length();
		char pad = getPadding(a);
		for(int i=0; i<a;++i){
			str+=pad;
		}
		return str;
	}
	
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
	//��������������е���Ӧ�������k��one��ƫ������
	private static void xorBytes(byte[] one,byte[] tmp,int k){
		for(int i = 0;i<16 ;++i){
			tmp[i] ^= one[k+i];
		}
	}
	//ֵת�ƺ�����ֻ��ʵ��һ������
	public static void giveValues(byte[] toHere,byte[] tmp,int k){
		for(int i = 0; i<16;i++){
			toHere[k+i]=tmp[i];
		}
	}
	//���ļ������е��ַ������������ҷ���
	public static String readAll(String fStr) throws IOException{
		BufferedReader bf = new BufferedReader(new FileReader(fStr));
		StringBuffer in = new StringBuffer();
		String tmp="";
		for(;(tmp=bf.readLine()) != null ; ){
			in.append(tmp);
		}

		return in.toString();
	}
	
	//���ø�����Կ��ȡ�������
	private static Cipher getCipher(byte[] keyBytes) throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException{
		Cipher cipher = Cipher.getInstance(CIPHER_ALG);
		Key key = toKey(keyBytes);
		cipher.init(Cipher.DECRYPT_MODE, key);
		return cipher;
	}

	//��byte[]����ת������Կ����
	private static SecretKey toKey(byte[] keyBytes){
		SecretKey sk = new SecretKeySpec(keyBytes,KEY_ALG);
		return sk;
	}
	//����ȫ0IVֵ
	private static String genZeroIv(){
		String iv = "";
		char ze = 0x0;
		for(int i=0;i<16;++i){
			iv+=ze;
		}
		return iv;
	}
	//base64�������
	public static String encode(byte[] input){
		
		
		byte[] data = Base64.encode(input);
		return new String(data);
	}
	//base64����
	public static byte[] decode(String input){
		
		byte[] data = Base64.decode(input);
		return data;
	}
	

}
