import java.util.LinkedHashMap;
import java.util.Map;

public class Set13_profile{
	static byte[] keyByte;
	static int uidCount = 10;
	
	public static void getKey() throws Exception{
		keyByte = AESCoder.initKey();
	}

	public static Map<String, String> parse(String pairs){
		Map<String, String> m = new LinkHashMap<String, String>();
		String[] pairArr = pairs.split("&");
		String[][] tmpArr = new String[pairArr.length][2];

		for(int i = 0; i < pairArr.length; i++){
			tmpArr[i] = pairArr[i].split("=");
		}

		for (int j = 0; j < pairArr.length; j++){
			if (tmpArr[j].length == 2)
				m.put(tmpArr[j][0], tmpArr[j][1]);
			else
				m.put(tmpArr[j][0], null);
		}

		return m;		
	}

	public static Map<String, Comparable> profile_for(String email){
		Map<String, Comparable> m = new LinkedHashMap<String, Comparable>();
		email = email.replaceAll("&", "");
		email = email.replaceAll("=", "");
		m.put("email", email);		
		m.put("uid", uidCount);
		m.put("role", "user");
		
		//System.out.println(m);


		return m;
	}

	public static String encoded(Map<?, ?> m){
		String result = "";
		for (Object key : m.keySet()){
			if (result != ""){
				result += "&";
			}

			result += key + "=" + m.get(key);
		}	

		return result;
	}

	public static byte[] encrypt(String email) throws Exception{
		String profile = encoded(profile_for(email));
		System.out.println(profile);
		byte[] cipher = AESCoder.encryptECB(profile.getBytes(), keyByte);

		for (byte a : cipher)
			System.out.print((char) a);
		System.out.println();
		return cipher;
	}

	public static void main(String[] args){
		
		Map<String, String> n = new LinkHashMap<String, String>();
		
		try {
			getKey();
			//System.out.println(normalPro);
			byte[] normalCipher = encrypt("sfsfsf@qq.com"); //指定邮箱产生序列的密文
			byte[] attackCipher = encrypt("sfsfsfsfsfadmin"); //填充后admin产生序列的密文

			byte[] proCipher = new byte[48];
			String proAdmin = "";
			
			int i = 0;
			//System.out.println((proCipher = emailBlock + roleBlock));
			for (i = 0; i < 48; i++){ 
				if (i >= 0 && i < 32){ //将normalCipher的前32Bytes截取
					proCipher[i] = normalCipher[i];
				}else if ( i >= 32 && i < 48){  //将attackCipher的第二段16Bytes截取，并同前一段拼接在一起
					proCipher[i] = attackCipher[i - 16];
				}
			}
			
			byte[] profile = AESCoder.decryptECB(proCipher, keyByte); //将结果解密
			for (int b = 0; b < profile.length; b++){
				proAdmin += (char)profile[b];
			
			}
			System.out.println(proAdmin);
			//System.out.println(parse(proAdmin));
			//n = parse(proAdmin);
	
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

			
	}
}