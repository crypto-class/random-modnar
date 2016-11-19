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
	 * ��ʼ�����趨�����˿�
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
	 * �����˿ڵȴ�����
	 */
	public void run(){
		while(cflag==false){	//��������
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
	 * �������ݰ�
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
	 * �������ݰ�
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
	 * ��Ӧ����
	 */
	public void reaction(){
		String respondString;
		while(true){
			byte[] data = null;
			
			//����
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
			
			//����
			if(data.length==2&& new String(data).contains("hi")){
				
				
				//��һ�λ�ȡ����
				respondString = "comment1=cooking%20MCs;userdata=1;comment2=%20like%20a%20pound%20of%20bacon";
				post(encrypt(respondString));
				
				
			}else{
				
				
				//��֤�Ƿ�admin
				String re = null;
				data = decrypt(data);
				String str;
				String[] subString;
				str = new String(data);
				subString = str.split(";");
				for(int i = 0; i < subString.length; i++){
					if(subString[i].split("=")[0].equals("admin")){
						if(subString[i].split("=")[1].equals("true")){
							//�ɹ�
							System.out.println("admit");
							re = "fin";
							post(re.getBytes());
							cflag=false;
							return;
						}
					}
				}
				
				
				//ʧ��
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
			//KeyGenerator ����aes�㷨��Կ  
			System.out.println("��Կ�ĳ���Ϊ��" + secretKey.getEncoded().length);  
          
			cipher.init(Cipher.ENCRYPT_MODE, secretKey, new IvParameterSpec(getIV()));//ʹ�ü���ģʽ��ʼ�� ��Կ  
			byte[] encrypt = cipher.doFinal(data.getBytes()); //�������ֲ������ܻ�������ݣ����߽���һ���ಿ�ֲ�����  
          
			System.out.println("���ܣ�" + Arrays.toString(encrypt));  
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
			cipher.init(Cipher.DECRYPT_MODE, secretKey, new IvParameterSpec(getIV()));//ʹ�ý���ģʽ��ʼ����Կ  
			byte[] decrypt = cipher.doFinal(data);
			System.out.println("���ܺ�" + new String(decrypt)); 
			return decrypt;
			
			
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}// ����������    
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
