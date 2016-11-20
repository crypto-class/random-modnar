package get_invitation;

import java.math.BigInteger;

import org.bouncycastle.util.encoders.Base64;

public class ATK {
	public static void main(String args[]){
		RSA rsa = new RSA();
		rsa.setN("402394248802762560784459411647796431108620322919897426002417858465984510150839043308712123310510922610690378085519407742502585978563438101321191019034005392771936629869360205383247721026151449660543966528254014636648532640397857580791648563954248342700568953634713286153354659774351731627683020456167612375777");
		BigInteger c1 = new BigInteger("239450055536579126410433057119955568243208878037441558052345538060429910227864196906345427754000499641521575512944473380047865623679664401229365345208068050995600248796358129950676950842724758743044543343426938845678892776396315240898265648919893384723100132425351735921836372375270138768751862889295179915967"
										,10);
		BigInteger c2 = new BigInteger("138372640918712441635048432796183745163164033759933692015738688043514876808030419408866658926149572619049975479533518038543123781439635367988204599740411304464710538234675409552954543439980522243416932759834006973717964032712098473752496471182275216438174279723470286674311474283278293265668947915374771552561"
										,10);
		BigInteger e1 = new BigInteger("3",10);
		BigInteger e2 = new BigInteger("10001",16);
		BigInteger m;
		BigInteger[] s;
		s=rsa.egcd(e1, e2);
		if(s[1].compareTo((new BigInteger("0"))) == -1){//-1为小于
			s[1] = s[1].abs();
			c1 = c1.modInverse(rsa.N);
		}
		if(s[2].compareTo((new BigInteger("0"))) == -1){//-1为小于
			s[2] = s[2].abs();
			c2 = c2.modInverse(rsa.N);
		}
		
		BigInteger t1,t2;
		t1 = c1.pow(s[1].intValueExact());
		t2 = c2.pow(s[2].intValueExact());
		m  = t1.multiply(t2); 
		m  = m.mod(rsa.N);
		String answer = RSA.hexStringToString(m.toString(16));
		answer = new String(Base64.decode(answer));
		System.out.println(answer);
	}
}

class RSA{
	BigInteger N;
	
	
	/**
	 * 设定N,十进制输入
	 * @param m
	 */
	public void setN(String m){
		N = new BigInteger(m,10);
	}
	
	public BigInteger[] egcd(BigInteger a, BigInteger b){
		BigInteger[] re = new BigInteger[3];
		if(a.equals(new BigInteger("0"))){
			re[0]=b;
			re[1]=new BigInteger("0");
			re[2]=new BigInteger("1");
			return re;
		}else{
			re = egcd(b.mod(a),a);
			BigInteger t = re[1];
			re[1] = re[2].subtract((b.divide(a).multiply(re[1])));
			re[2] = t;
			return re;
		}
		
	}
	
	/**
	 * 十六进制转字符串
	 * @param s
	 * @return
	 */
	public static String hexStringToString(String s) {  
        if (s == null || s.equals("")) {  
            return null;  
        }  
        s = s.replace(" ", "");  
        byte[] baKeyword = new byte[s.length() / 2];  
        for (int i = 0; i < baKeyword.length; i++) {  
            try {  
                baKeyword[i] = (byte) (0xff & Integer.parseInt(  
                        s.substring(i * 2, i * 2 + 2), 16));  
            } catch (Exception e) {  
                e.printStackTrace();  
            }  
        }  
        try {  
            s = new String(baKeyword, "gbk");  
            new String();  
        } catch (Exception e1) {  
            e1.printStackTrace();  
        }  
        return s;  
    }  
	
	 
}