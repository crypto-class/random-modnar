
#include "fun.h"

string char2Base(char cc, int base, int n)
{
	int c = ((cc - '\0') % 256 + 256) % 256;
	string s;s.clear();
	while (n--)
	{
		int d = c%base;
		if (d>9)
			s += d + 'a' - 10;
		else
			s += d + '0';
		c /= base;
	}
	s = string(s.rbegin(), s.rend());
	return s;
}
string hex2str(string s)
{
	string ret;
	for (int i = 0;i<(int)s.length();i += 2)
	{
		int t = 0;
		t += (s[i + 1] <= '9' && s[i + 1] >= '0') ? s[i + 1] - '0' : s[i + 1] - 'a' + 10;
		t += 16 * ((s[i] <= '9' && s[i] >= '0') ? s[i] - '0' : s[i] - 'a' + 10);
		char c = (t + '\0');
		ret += c;
	}
	return ret;
}

string str2hex(string s)
{
	string hex;hex.clear();
	for (int i = 0;i<(int)s.length();i++)
	{
		hex += char2Base(s[i], 16, 2);
		hex += " ";
	}
	return hex;
}

string AXORB(string a, string b)
{
	int len = min(a.length(), b.length());
	string s;s.clear();s.resize(len);
	for (int i = 0;i<len;i++)
		s[i] = a[i] ^ b[i];
	return s;
}

string AXORWord(string a, string word)
{
	int len = a.length();
	int len2 = word.length();
	string cipher; cipher.clear();
	for (int i = 0;i < len;++i)
	{
		cipher += (a[i] ^ word[(i%len2)]);
	}
	return cipher;
}

//void str2byte(string a,byte b[])
//{
//
//}

/**********kmp**********/
void kmp_pre(string x, int m, int nxt[])
{
	int i, j;
	j = nxt[0] = -1;
	i = 0;
	while (i<m)
	{
		while (j != -1 && x[i] != x[j]) j = nxt[j];
		nxt[++i] = ++j;
	}
}
int nxt[10010];
int kmp_count(string pa, int m, string s, int n)
{
	memset(nxt, 0, sizeof(nxt));
	int i, j;
	int ans = 0;
	kmp_pre(pa, m, nxt);
	i = j = 0;
	while (i<n)
	{
		while (j != -1 && s[i] != pa[j])
			j = nxt[j];
		i++, j++;
		if (j >= m)
		{
			ans++;
			j = nxt[j];
		}
	}
	return ans;
}

int CountBitOne(char a)
{
	int cnt = 0;
	for (int i = 0;i < 8;++i)
	{
		cnt += (a & 1);
		a >>= 1;
	}
	return cnt;
}

int HammingDist(string a, string b)
{
	string di = AXORB(a, b);
	int len = di.length();
	int dis = 0;
	for (int i = 0;i < len;++i)
	{
		dis += CountBitOne(di[i]);
	}
	return dis;
}

double CountFrequencySTD(string a)
{
	int cnt[26] = {};
	double std = 0;
	int len = a.length();
	int alp_cnt = 0;
	for (int i = 0;i < len;++i)
	{
		char c = a[i];
		if ((c <= 'Z' && c >= 'A') || (c >= 'a' && c <= 'z'))
		{
			if ((c <= 'Z' && c >= 'A'))
				c = c - 'A' + 'a';
			cnt[c - 'a']++;
			alp_cnt++;
		}
	}
	//cout << "alphabet frequency = " << endl;
	double tot = 0;
	for (int i = 0;i < 26;i++)
	{
		double q = (cnt[i] / (double)alp_cnt);
		//	printf_s("%f ", q);tot += q;
		q *= alphabetFre[i];
		std += q;
	}
	//printf_s("q = %f   ", std);
	//cout << tot << endl;
	return std;
}

string Base64Enc(string strSrc)
{
	string strOut;
	CryptoPP::StringSink* sink = new CryptoPP::StringSink(strOut);
	Base64Encoder *base64Enc = new Base64Encoder(sink);
	int iLen = strSrc.size();
	StringSource source(strSrc, true, base64Enc);

	int iPos = -1;
	iPos = strOut.find("\n", 0);
	while (iPos >= 0)
	{
		strOut = strOut.erase(iPos, 1);
		iPos = strOut.find("\n", 0);
	}

	return strOut;
}
string Base64Dec(string strSrc)
{
	string strOut;
	StringSink *sink = new StringSink(strOut);
	Base64Decoder *baseDec = new Base64Decoder(sink);
	StringSource dst(strSrc, true, baseDec);
	return strOut;
}

string ECB_AES_DEC(string cipher, string key)
{
	byte aes_key[AES::DEFAULT_KEYLENGTH];
	memcpy(aes_key, key.data(), key.length());

	string plain;

	ECB_Mode< AES >::Decryption Decryptor(aes_key, AES::DEFAULT_KEYLENGTH);
	StringSource(cipher, true, new StreamTransformationFilter(Decryptor, new StringSink(plain), BlockPaddingSchemeDef::BlockPaddingScheme::PKCS_PADDING, true));
	return plain;

}

/*
¼ÓÃÜÇ°Òªpadding
*/
string ECB_AES_ENC(string plain, string key)
{
	byte aes_key[AES::DEFAULT_KEYLENGTH];
	memcpy(aes_key, key.data(), key.length());

	string cipher;

	ECB_Mode< AES >::Decryption Encryptor(aes_key, AES::DEFAULT_KEYLENGTH);
	StringSource(plain, true, new StreamTransformationFilter(Encryptor, new StringSink(cipher), BlockPaddingSchemeDef::BlockPaddingScheme::PKCS_PADDING, true));
	return cipher;
}

string PKSC_7(string s, int blocksize)
{
	int num = blocksize - (int)s.length() % blocksize;
	string ret = s;
	for (int i = 0;i < num;++i)
	{
		ret += (num + '\0');
	}
	return ret;
}

string CBC_AES_DEC(string cipher, string key, byte* iv)
{
	byte aes_key[AES::DEFAULT_KEYLENGTH];
	memcpy(aes_key, key.data(), key.length());

	string plain;

	CBC_Mode< AES >::Decryption Decryptor(aes_key, AES::DEFAULT_KEYLENGTH, iv);

	StringSource(cipher, true, new StreamTransformationFilter(Decryptor, new StringSink(plain)));
	return plain;
}

string CBC_AES_ENC(string plain, string key, byte* iv)
{
	byte aes_key[AES::DEFAULT_KEYLENGTH];
	memcpy(aes_key, key.data(), key.length());

	string cipher;

	CBC_Mode< AES >::Encryption Encryptor(aes_key, AES::DEFAULT_KEYLENGTH, iv);

	StringSource(plain, true, new StreamTransformationFilter(Encryptor, new StringSink(cipher)));
	return cipher;
}

string CBC_AES_DEC(string cipher, byte aes_key[], byte* iv)
{
	string plain;

	CBC_Mode< AES >::Decryption Decryptor(aes_key, AES::DEFAULT_KEYLENGTH, iv);

	StringSource(cipher, true, new StreamTransformationFilter(Decryptor, new StringSink(plain)));
	return plain;
}

string CBC_AES_ENC(string plain, byte aes_key[], byte* iv)
{
	string cipher;

	CBC_Mode< AES >::Encryption Encryptor(aes_key, AES::DEFAULT_KEYLENGTH, iv);

	StringSource(plain, true, new StreamTransformationFilter(Encryptor, new StringSink(cipher)));
	return cipher;
}

vector<string> chunk(string s, int blocksize)
{
	int len = s.length();
	vector<string> ret;
	ret.clear();
	for (int i = 0;i + blocksize < len;i += blocksize)
	{
		ret.pb(s.substr(i, blocksize));
	}
	return ret;
}

Integer powMod(Integer a, Integer n, Integer mod)
{
	Integer ans = 1;
	while (n != 0)
	{
		if (n % 2 == 1)
			ans = (ans*a) % mod;
		a = (a*a) % mod;
		n /= 2;
	}
	return ans;
}

Integer Byte2Inter(byte b[], int n)
{
	Integer ans = 0;
	Integer to = 1;
	for (int i = n - 1;i >= 0;--i)
	{
		byte a = b[i];
		for (int j = 0;j < 8;j++)
		{
			if (a & 1)
				ans += to;
			a >>= 1;
			to *= 2;
		}
	}
	return ans;
}
void Inter2Byte(Integer a, byte b[], int n)
{
	int seg = 0;
	for (int i = 0;i < n;++i)
	{
		int pos = BYTELEN;
		byte tmp = 0;
		int two = 1;
		while (pos--)
		{
			if (a % 2 == 1)
				tmp += two;
			two *= 2;
			a /= 2;
		}
		b[seg++] = tmp;
	}
}

string Inter2Str(Integer a)
{
	string s = "";
	while (a != 0)
	{
		int t = a % 10;
		s += ('0' + t);
		a /= 10;
	}
	reverse(s.begin(), s.end());
	return s;
}

Integer Str2Inter(string a, int base)
{
	int len = a.length();
	Integer tmp = 1;
	Integer ans = 0;
	for (int i = len - 1;i >= 0;i--)
	{
		char c = a[i];
		int d;
		if (c <= '9'&&c >= '0')
		{
			d = c - '0';
		}
		else if (c <= 'z' && c >= 'a')
		{
			d = c - 'a' + 10;
		}
		else if (c <= 'Z' && c >= 'A')
		{
			d = c - 'A' + 10;
		}
		else {
			cout << "ERROR, NOT A VAILID NUMBER" << endl;
			exit(1);
		}
		ans += d*tmp;
		tmp *= base;
	}
	return ans;
}

double Inte2Doub(Integer a)
{
	double ans = 0;
	long long ten = 1;
	while (a != 0)
	{
		ans += ten*(a % 10);
		a /= 10;
		ten *= 10;
	}
	return ans;
}