package p16a;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class Attacker {
	public static void main(String[] args){
		Send s = new Send();
		s.start();
		String t = "hi";
		s.post(t.getBytes());
	}
}

class Send extends Thread{
	boolean cflag=false;
	private Socket s;
	private ObjectOutputStream os;
	private ObjectInputStream is;
	
	/**
	 * ��ʼ�������ӷ����
	 */
	public Send(){
		try {
			s=new Socket("127.0.0.1", 6623);
			if(s!=null){
				cflag=true;
				os=new ObjectOutputStream(s.getOutputStream());
				is=new ObjectInputStream(s.getInputStream());	
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * �����յ��İ�
	 */
	public void run(){
		while(cflag==true){
			try {
				synchronized(is){
					byte[] data = (byte[]) is.readObject();
					Thread.sleep(1);
					judge(data);
				}
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return;
			}
		}
	}
	
	/**
	 * ���յ��İ����ͽ����жϺʹ���
	 * @param data
	 */
	private void judge(byte[] data){
		if(data.length == 3){
			System.out.println("success");
			cflag = false;
		}
		else if(data.length == 4){
			System.out.println("fail");
			cflag = false;
		}
		else{
			atk(data);
			post(data);
		}
	}
	
	/**
	 * ��������
	 * @param data
	 * @return
	 */
	private void atk(byte[] data){
		String str = "comment1=cooking%20MCs;userdata=0102030405060708;comment2=%20like%20a%20pound%20of%20bacon";//��֪ԭ��
		int len = str.length();
		
		
		byte[] origin = str.getBytes();
		len = 33;//�޸�λ���趨,�޸ĵ����飬���Ƶ��Ŀ�
		
		str="admin=true;c=";//Ҫ�޸ĳɵ��ַ���
		byte[] fix = str.getBytes();
		
		for(int i = 0; i < fix.length; i++){
			/*
			 * ����c[i],��Ӧ����p1[i]��Ŀ������p2[i]
			 * c[i] = c[i]^p1[i]^p2[i]
			 */
			data[len + i] = (byte) (data[len + i] ^ origin[len + 16 + i] ^ fix[i]);
		}
	}
	
	/**
	 * ��������
	 * @param data
	 */
	public void post(byte[] data){
		try {
			os.writeObject(data);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
