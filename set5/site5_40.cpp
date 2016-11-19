#include <string>
#include "fun.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

using namespace CryptoPP;
using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)

//FILE *fi = freopen("data.in", "r", stdin);
//FILE *fi = freopen("test.in", "r", stdin);
//FILE *f2 = freopen("data.out", "w", stdout);

#define SECURELEN 32
//FILE *fo = freopen("result.out", "w", stdout);



/*
假如同一的明文用不同的私钥对RSA多次进行加密
那么用
中国剩余定理
很容易进行破解
当然，，前提是e是一样的情况下。。。
M^e = C1 mod N1
M^e = C2 mod N2
M^e = C3 mod N3
....
*/

Integer ChineseRemainderTheory(Integer Ni[], Integer Ai[], int n)
{
	Integer M = 1;
	Integer ans = 0;
	for (int i = 0;i < n;i++)
		M *= Ni[i];
	for (int i = 0;i < n;i++)
	{
		Integer mi, tmp;
		Integer Mi = M / Ni[i];
		exgcd(Mi, Ni[i], mi, tmp);
		ans = (ans + Ai[i] * mi*Mi) % M;
	}
	return ans;
}

const int num = 3;
Integer p[num], q[num], phi[num], n[num], e[num], d[num], mes[num];
Integer m = 12521, c[num];

InvertibleRSAFunction params[num];

AutoSeededRandomPool rnd;
int main()

{
	srand(time(0));

	//Integer xx, mm[3], aa[3];
	//mm[0] = 3, mm[1] = 5, mm[2] = 7;
	//aa[0] = 2, aa[1] = 3, aa[2] = 2;
	//xx = ChineseRemainderTheory(mm, aa, 3);
	//cout << xx << endl;


	cout << "m = " << m << endl;
	for (int i = 0;i < num;i++)
	{
		//	cout << "--------------------" << endl;
		params[i].GenerateRandomWithKeySize(rnd, SECURELEN);
		p[i] = params[i].GetPrime1();
		q[i] = params[i].GetPrime2();
		n[i] = p[i] * q[i];
		phi[i] = (p[i] - 1)*(q[i] - 1);
		//	cout << "p = " << p[i] << endl;
		//	cout << "q = " << q[i] << endl;
		e[i] = 3;
		//e[i] = params[i].GetPublicExponent();
		d[i] = params[i].GetPrivateExponent();
		//	cout << "e = " << e[i] << endl;
		//	cout << "d = " << d[i] << endl;
		c[i] = powMod(m, e[i], n[i]);
		mes[i] = powMod(c[i], d[i], n[i]);
		cout << "c[" << i << "]" << " = " << c[i] << endl;
		//	cout << "mes[" << i << "]" << " = " << mes[i] << endl;
	}
	Integer plain = ChineseRemainderTheory(n, c, num);
	double ans = Inte2Doub(plain);
	cout << plain << endl;
	cout << ans << endl;
	ans = pow(ans, 1.0 / 3);
	cout << "attach plain = " << ans << endl;


	system("pause");
	return 0;
}

/*
https://en.wikipedia.org/wiki/Chinese_remainder_theorem

Reuslt:

m = 12521.
c[0] = 2886790063.
c[1] = 3427718147.
c[2] = 2298636027.
1962985296761.
1.96299e+12
attach plain = 12521
*/