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
				//�õ�AES�Ŀ�Ĵ�С
				if(isECB()){
					//�Ƿ���ECBģʽ
					byte[] re = tryToGetPlaintext(in);
					//get������Ϣ
					System.out.println(new String(re));
					//��������Ϣ��ӡ������̨
					
				}else{
					//����EBCģʽ�Ļ��������κδ���
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
			//�������ں��������Ǽ���һ�£����һ��getһ��������ģ�Ҫѭ�����ٴ���get�ꡣȻ��ÿ�ζ��ѿ���Ȩ����get1Block()
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
				//ÿ��getһ����Ĵ�С�����������
				for(int k = 0 ; k < 16 && reIndex < in.length;reIndex++,k++){
					result[reIndex] = temp[k];
				}
			}
			return result;
			
		}
		public static byte[] get1Block(byte[] plaintext,int shift) throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException, NoSuchProviderException{
			//�������ģ���ƫ������ƫ��������˼�Ǵ����λ�ÿ�ʼ�������ģ�С����
			//��ʼ��test
			byte[] test =  new byte[16];
			for(int i = 0;i<16;++i){
				test[i]=(byte)'A';
			}
			int testIndex = test.length-1;
			
			byte[] result = new byte[16];
			int reIndex = 0;
			
			for(;reIndex<16;reIndex++,testIndex--){
				//��ʼ��test�������ݣ�ÿ�ζ�Ҫ��ʱ�����Ѿ������������
				for(int k=0;k<reIndex;++k){
					test[k+testIndex]=result[k];
				}
			
				//��ʼ�����Ĺ���
				byte[] p = new byte[plaintext.length+16];
				for(int k =0;k<testIndex+shift;k++){
						p[k]=(byte)'A';
				}
				//��ʱ�������ĵĹ��죬�൱��ÿ�γ���һ���ֽڣ�Ȼ����Ǹ��ֽڷ������������У�Ȼ�������¹���
				for(int k =testIndex+shift,j=0;j+shift < plaintext.length ;++k,j++){//&& testIndex+k < p.length
					p[k]=plaintext[j+shift];
				}
				//get��һ���ֽڵ�����
				result[reIndex] = getLast1ByteIn1Block(p,test,shift);
			}
			return result;
		}
		
		
		public static byte getLast1ByteIn1Block(byte[] plaintext ,byte[] test,int shift) throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException, NoSuchProviderException{
			//getһ���ֽڵ�����
			//shiftβ���Ƶ�ƫ��λ�ã�ֻ���Ƹĳ������һ���ֽڼ�shift+15��Ҳ������˵��С����
			
			Cipher c = getCipher(util.genAESKey());
			byte[] plain = util.autoPKCS7Padding(plaintext);
			byte[] res = c.doFinal(plain);
			boolean flag = false;
			for(int i = 0 ;i<256 ; ++i){
				test[15]=(byte)i;
				//ÿ�θ���test�����һ���ֽ�
				byte[] testCipher = c.doFinal(test);
				for(int k = 0;k<16;++k){
					//ѭ���ж��Ƿ�byte�����Ƿ����
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
			//������Ʋ���������ȫ1����Ȼ�������ܱ��Ʋ�����
			return (byte)0xff;
		}
		
		
		public static boolean isECB() throws InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException, NoSuchProviderException{
			//�ж��Ƿ���EBCģʽ���õ�blocksize->��������ͬ���ݵķ�����ܣ����������ͬ���ǣ�����ͬ�Ͳ���
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
			//�õ�AES�����С������һ��һ�����ӣ���ʹ���κ�padding�����û�б����϶������ˡ����β�����Ĳ�ֵ���Ƿ���Ĵ�С
			Cipher c = getCipher(util.genAESKey());
			int b=0,d=0;
			boolean flag=false;
			for(int i = 0;true;){
				//��ǰ��i��A�ĺ���
				byte[] use = addABefore(in,i);
				try {
					c.doFinal(use);
				} catch (IllegalBlockSizeException | BadPaddingException e) {
					//�����ʱ��ѭ������
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
			//��ȫ�ֱ�������һ��
			blockSize=d-b;
			return d-b;
		}
	
		
		public static byte[] AESEBCEncrypto(byte[] inBytes,byte[] key) throws Exception{
			//ʵϰAES,CBCģʽ����
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
			//��tmp��16����ֵ��toHere��kΪƫ��
			for(int i = 0; i<16;i++){
				toHere[k+i]=tmp[i];
			}
		}
		
		private static Key toKey(byte[] keyStr){
			SecretKey sk = new SecretKeySpec(keyStr,KEY_ALG);
			return sk;
		}
		
		public static byte[] addABefore(byte[] data,int len){
			//���len��A������ǰ��
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
