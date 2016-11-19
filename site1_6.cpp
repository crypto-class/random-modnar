#include <string>
#include "fun.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/des.h"
#include "cryptopp/base64.h"
#include "cryptopp/md5.h"
#include "cryptopp/hex.h"
#include "cryptopp/files.h"
#include "test.h"
using namespace CryptoPP;
using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)

FILE *fi = freopen("site1_6.in", "r", stdin);
FILE *fo = freopen("site1_6.out", "w", stdout);

struct keylen
{
	int len;
	int dis;
};
vector<keylen>K;

int Kasiski(string s,int block_size)
{
	int dis = 0;
	for (int i = 0;i + block_size < (int)s.length(); ++i)
	{
		if (s[i] == s[i + block_size])
			dis++;
	}
	return dis;
}

double std_qsum = 0.065;
bool cmp(keylen a, keylen b)
{
	return a.dis > b.dis;
}

string Base64Dec(string strSrc)
{
	string strOut;
	StringSink *sink = new StringSink(strOut);
	Base64Decoder *baseDec = new Base64Decoder(sink);
	StringSource dst(strSrc, true, baseDec);
	return strOut;
}


struct  candChar
{
	char c;
	double det;
	bool operator < (const candChar &b) const
	{
		return det < b.det;
	}
};

string FindSingleKey(int step, string s, int keylen)
{
	cout << "follow is pos = " << step;
	string ret;
	vector<candChar>K;
	for (int cc = 0x20;cc <= 0x7e;cc++)
	{
		char c = cc + '\0';
		int flag = 1;
		string refer = "";
		for (int i = step - 1;i < (int)s.length(); i += keylen)
		{
			char nw = s[i] ^ c;
			if (!(nw == '\n' || (nw <= 0x7e && nw >= 0x20)))
			{
				flag = 0;
				break;
			}
			refer += nw;
		}
		if (flag)
		{
			double p = CountFrequencySTD(refer);
			K.pb(candChar{c,p});
		}
	}
	sort(K.begin(), K.end());
	for (auto x : K)
	{
		ret += x.c;
		ret += " ";
	}
	cout << "char maybe is " << endl;
	return ret;
}


int main()
{
	printf("%x\n", '\n');
	string test1 = "this is a test";
	string test2 = "wokka wokka!!!";
	cout << HammingDist(test1, test2) << endl;


	string cipher, s;
	while (cin>>s)
	{
		s = Base64Dec(s);
		cipher += s;
	}
	cout << cipher.length() << endl;
	//cout << cipher << endl;


	int length = 1;
	int maxCount = 0;
	for (int step = 2;step <= 30; step++)
	{
		int cnt = Kasiski(cipher, step);
		K.pb(keylen{ step,cnt });
		if (cnt > maxCount)
		{
			maxCount = cnt;
			length = step;
		}
	}
	sort(K.begin(), K.end(), cmp);
	for (int i = 0;i < (int)K.size();++i)
	{
		printf_s("step = %d, cnt = %d\n", K[i].len,K[i].dis);
	}
	cout << length << endl;

	for (int step = 1;step <= length; ++step)
	{
		string s = FindSingleKey(step,cipher,length);
		cout << s << endl;
	}


	string Key = "Terminator X: Bring the noise";

	string plain = AXORWord(cipher, Key);
	cout << plain << endl;


	STOP;/*
	while (true)
	{
		;
	}*/
	return 0;
}
