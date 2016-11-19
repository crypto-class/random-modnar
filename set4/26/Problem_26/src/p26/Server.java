package p26;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;

public class Server {
	public static void main(String[] args){
		Response r = new Response();
		r.start();
		r.reaction();
	}
}

class Response extends Thread{
	boolean cflag=false;	//is connecting
	private ServerSocket ss;
	private ObjectOutputStream os;
	private ObjectInputStream is;
	SecretKey secretKey;

	
	/**
	 * 初始化，设定监听端口
	 */
	public Response(){ 
		try {
			secretKey = KeyGenerator.getInstance("AES").generateKey(); 
			ss=new ServerSocket(6623);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * 监听端口等待连接
	 */
	public void run(){
		while(cflag==false){	//建立连接
			Socket socket=null;
			try {
				socket=ss.accept();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				continue;
			}
			try{
			os=new ObjectOutputStream(socket.getOutputStream());
			is=new ObjectInputStream(socket.getInputStream());	
			cflag=true;
			}catch(IOException e){}
		}
	}
	
	/**
	 * 发送数据包
	 */
	private void post(byte[] data){
		synchronized(os){
			try {
				os.writeObject(data);
				os.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * 接收数据包
	 */
	private byte[] receive(){
		byte[] data = null;
		synchronized(is){
			try {
				data = (byte[])is.readObject();
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return data;
	}
	
	/**
	 * 响应规则
	 */
	public void reaction(){
		String respondString;
		while(true){
			byte[] data = null;
			
			//监听
			while(data == null){
				try{
					if(cflag == true){
						data = receive();
					}
					Thread.sleep(1);
				}catch(Exception e){
					cflag = false;
					return;
				}
			}
			
			//处理
			if(data.length==2&& new String(data).contains("hi")){
				
				
				//第一次获取密文
				respondString = "comment1=cooking%20MCs;userdata=1;comment2=%20like%20a%20pound%20of%20bacon";
				post(encrypt(respondString));
				
				
			}else{
				
				
				//验证是否admin
				String re = null;
				data = decrypt(data);
				String str;
				String[] subString;
				str = new String(data);
				subString = str.split(";");
				for(int i = 0; i < subString.length; i++){
					if(subString[i].split("=")[0].equals("admin")){
						if(subString[i].split("=")[1].equals("true")){
							//成功
							System.out.println("admit");
							re = "fin";
							post(re.getBytes());
							cflag=false;
							return;
						}
					}
				}
				
				
				//失败
				if(re == null){
					re = "fail";
					post(re.getBytes());
					cflag=false;
					return;
				}
				
			}
			
			
		}
	}
	
	static byte[] getIV() {  
        String iv = "1234567812345678"; //IV length: must be 16 bytes long  
        return iv.getBytes();  
    }  
	
	private byte[] encrypt(String data){
		try{
			Cipher cipher = Cipher.getInstance("AES/CTR/PKCS5Padding");  
			//KeyGenerator 生成aes算法密钥  
			System.out.println("密钥的长度为：" + secretKey.getEncoded().length);  
          
			cipher.init(Cipher.ENCRYPT_MODE, secretKey, new IvParameterSpec(getIV()));//使用加密模式初始化 密钥  
			byte[] encrypt = cipher.doFinal(data.getBytes()); //按单部分操作加密或解密数据，或者结束一个多部分操作。  
          
			System.out.println("加密：" + Arrays.toString(encrypt));  
			decrypt(encrypt);
			return encrypt;
		}catch(Exception e){
			e.printStackTrace();
		}
        return null;
	}
	
	
	private byte[] decrypt(byte[] data){
		Cipher cipher;
        try {
        	
			cipher = Cipher.getInstance("AES/CTR/PKCS5Padding");
			cipher.init(Cipher.DECRYPT_MODE, secretKey, new IvParameterSpec(getIV()));//使用解密模式初始化密钥  
			byte[] decrypt = cipher.doFinal(data);
			System.out.println("解密后：" + new String(decrypt)); 
			return decrypt;
			
			
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}// 创建密码器    
        catch (InvalidKeyException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalBlockSizeException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (BadPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InvalidAlgorithmParameterException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        return null;
	}
}
