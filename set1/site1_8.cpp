#include <string>
#include "fun.h"

using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)

FILE *fi = freopen("site1_8.in", "r", stdin);
FILE *fo = freopen("site1_8.out", "w", stdout);
/*
ECB模式容易导致不同组可能会有相同的密文
通过这一特性统计即可
*/
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
/*
计算重叠个数
*/
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
	string cipher, s;
	vector<Cipher>C;
	cout << "site1_8" << endl;
	int cnt = 0;
	while (cin >> s)
	{
		if (cnt == 132)
		{
			STOP;
		}
		s = hex2str(s);
		C.pb(Cipher{ s,HavingOverlapping(s), cnt++ });
	}
	sort(C.begin(), C.end());
	cout << "maybe follow cipher are encrypt by EBC" << endl;
	cout << C[cnt - 1].s << endl;
	cout << str2hex(C[cnt - 1].s) << endl;



	/*while (true)
	{
	;
	}*/
	return 0;
}
