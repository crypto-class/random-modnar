package letter;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class Letter {
	public static void main(String[] args){
		Crypto c = new Crypto(new byte[7]);
		try {
			c.searchKey(0, new String());
			c.fo.close();
			c.fi.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}

class Crypto{
	byte[] key;
	byte[] message;
	FileInputStream fi;
	FileOutputStream fo;
	boolean RoundStart = false;//ƥ��
	boolean IsAccept = true;
	
	
	/**
	 * ������Կ
	 * @param key
	 */
	public Crypto(byte[] key){
		this.key=key;
		message = new byte[833];
		try {
			fi = new FileInputStream("E:mtc3-esslinger-17-cipher-en.txt");
			fo = new FileOutputStream("E:letter.txt");
			fi.read(message);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * ��key����Ϊ7ʱ����
	 * @param str
	 * @return
	 */
	byte[] decrypt(byte[] str){
		if(key.length!=7){
			return null;
		}
		

		byte[] result = new byte[7];
		for(int i=0;i<7;i++){
			result[i] = str[key[i]];
		}
		return result;
	}
	
	/**
	 * ���ļ�ִ���ƽ����
	 * @param filename
	 */
	void decrypt(){
		byte[] buf = new byte[7];
		try {
			RoundStart = true;
			IsAccept = true;
			for(int i = 0; i < message.length; i=i+7){
				System.arraycopy(message, i, buf, 0, 7);
				buf = decrypt(buf);
				if(RoundStart == true){
					RoundStart = false;
					
					/**
					 * �ֶ���ӵ�ɸѡ����,ͨ����һ��ɸѡ
					 */
					
					/*��һ����ɸ���������,��Ϊǰ7��ֻ��һ����д��ĸT*/
					/*if(buf[0]!=(byte)'T'){
						IsAccept = false;
						break;
					}*/
					
					/*�ڶ�����ɸ���������*/
					byte[] jud = new byte[5];
					System.arraycopy(buf, 0, jud, 0, 5);
					if(!(new String(jud).equals("Today"))){
						IsAccept = false;
						break;
					}else{
						IsAccept = true;
					}
					
				}
				fo.write(buf);
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
	
	/**
	 * ������Կ
	 * @param key
	 * @param loc
	 * @throws IOException 
	 */
	void searchKey(int loc, String sav) throws IOException{
		if(loc == 7){
			decrypt();
			if(IsAccept){
				fo.write("\n".getBytes());
			}
			return ;
		}
		for(int i = 0;i < 7;i++){
			if(sav.contains(String.valueOf(i))){
				continue;
			}
			key[loc] = (byte)i;
			String nSav = sav+ String.valueOf(i);
			searchKey(loc+1, nSav);
			if(loc == 0){
				loc = 0;
			}
		}
	}
}
