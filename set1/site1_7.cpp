#include <string>
#include "fun.h"

using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)

FILE *fi = freopen("data.in", "r", stdin);
FILE *fo = freopen("data.out", "w", stdout);

int boom(string a, string b)
{
	for (int i = 0;i < (int)a.length() - 3;++i)
	{
		string aa = a.substr(i, 3);
		int flag = kmp_count(aa, 3, b, b.length());
		if (flag)
			return 1;
	}
	return 0;
}

int HavingOverlapping(string a)
{
	int ans = 0;
	for (int i = 0;i < (int)a.length();i += 16)
	{
		string aa = a.substr(i, 16);
		for (int j = i + 16;j < (int)a.length();j += 16)
		{
			string bb = a.substr(j, 16);
			ans += boom(aa, bb);
		}
	}
	return ans;
}

struct Cipher
{
	string s;
	int overlap, id;
	bool operator < (const Cipher & b) const
	{
		return overlap < b.overlap;
	}
};

int main()
{
	string cipher, s,plain;
	string key = "YELLOW SUBMARINE";
	while (cin >> s)
	{
		s = Base64Dec(s);
		//cout << s << endl;
		cipher += s;
	}
	plain = ECB_AES_DEC(cipher, key);
	cout << plain << endl;



	/*while (true)
	{
	;
	}*/
	return 0;
}
