package org.cryptopals.set2;

public class nine {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		String str = "YELLOW SUBMARINE";
		
		System.out.println(new String(padding(str.getBytes(), 20)));
		
	}
	

	//对数据进行填充
	public static byte[] padding(byte[] str,int len){
		int a = len - str.length;
		char pad = getPadding(a);
		byte[] out = new byte[len];
		for(int i=0; i<str.length;++i){
			out[i]=str[i];
		}
		for(int i=0; i<a;++i){
			out[str.length+i]=(byte)pad;
		}
		return out;
	}
	//得到padding填充对象
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

}
