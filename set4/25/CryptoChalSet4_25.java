import java.io.*;

import sun.misc.BASE64Decoder;

public class CryptoChalSet4_25{

	static String filePath = "25.txt";	
	final static byte[] iv = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p'};
	static byte[] keyByte;

	public static byte[] getText(String filePath) throws Exception{ //获取文件内容
		String filetext = "";
		String blockText = "";
		byte[] byteText = null;
		File f = new File(filePath);

		try{
			if (f.isFile() && f.exists()){
				InputStreamReader read = new InputStreamReader(
						new FileInputStream(f));
				BufferedReader bfr = new BufferedReader(read);

				while ((blockText = bfr.readLine()) != null){
					filetext += blockText;
				}

				BASE64Decoder decoder = new BASE64Decoder();
				byteText = decoder.decodeBuffer(filetext); 
			}else{
				System.out.println("File is not exist.");
			}	
		}catch(Exception e){
			e.printStackTrace();
		}

		return AESCoder.decryptECB(byteText, "YELLOW SUBMARINE".getBytes());

	}

	public static byte[] edit(byte[] ciphertext, int offset, byte[] newtext) throws Exception{
		byte[] newCiphertext = new byte[ciphertext.length];
		byte[] plaintext = AESCoder.decryptCTR(ciphertext, keyByte, iv); //密文解密得到明文
		for (int i = 0; i < ciphertext.length; i++){ //将offset指定处替换成newtext
			if (i >= offset && i < offset + newtext.length){
				newCiphertext[i] = (byte) (ciphertext[i] ^ plaintext[i] ^ newtext[i - offset]);
			}else{
				newCiphertext[i] = ciphertext[i];
			}
		}
		return newCiphertext;
	}

	public static byte[] attack(byte[] ciphertext) throws Exception{
		int cLength = ciphertext.length;
		byte[] tmpByteArray = new byte[cLength];
		byte[] tmpResultArray = null;
		byte[] possiblePlain = new byte[cLength];

		for (byte i : tmpByteArray){  //生成全零串
			i = '0';
		}

		tmpResultArray = edit(ciphertext, 0, tmpByteArray);  //调用edit()函数，生成新密文
		for (int j = 0; j < cLength; j++){
			possiblePlain[j] = (byte) (ciphertext[j] ^ tmpResultArray[j]); //新旧密文相异或
		}

		return possiblePlain;
	}

	public static void main(String[] args){
		byte[] newCipher;
		//byte[] newPlain = {'h', 'a', 't', 'e'};
		byte[] allNewP;
		byte[] attackResult;

		try {
			//key = "111111111";
			keyByte = AESCoder.initKey();
					
		    byte[] inputData = getText(filePath);
		    byte[] result = AESCoder.encryptCTR(inputData, keyByte, iv); //获取密文

		    attackResult = attack(result); //破解明文
		    for (byte i : attackResult){   //得到原明文
			    System.out.print((char)i);
		    }

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}