#include <string>
#include "fun.h"

using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)

FILE *fi = freopen("site1_7.in", "r", stdin);
FILE *fo = freopen("site1_7.out", "w", stdout);

/*
这真的没有必要注释了
*/

int main()
{
	string cipher, s, plain;
	string key = "YELLOW SUBMARINE";
	while (cin >> s)
	{
		s = Base64Dec(s);
		//cout << s << endl;
		cipher += s;
	}
	cout << cipher.size() << endl;
	cout << key.size() << endl;
	plain = ECB_AES_DEC(cipher, key);
	cout << plain << endl;



	/*while (true)
	{
	;
	}*/
	return 0;
}
